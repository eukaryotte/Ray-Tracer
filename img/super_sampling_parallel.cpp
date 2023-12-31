#include <memory>
#include <random>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "aggregate.h"

// メルセンヌ・ツイスタを乱数生成器として使用する
std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}

int main() {
    // スーパーサンプリング数
    const int N = 1000;

    Image img(512, 512);
    PinholeCamera cam(Vec3(0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(3, 0, -5), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(-5, 0, -7), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    Vec3 sunDir = normalize(Vec3(1, 1, 1));

    // for 文を並列化する
    // schedule(dynamic, 1)を指定すると動的に計算ブロックが割り当てられる
    #pragma omp parallel for schedule(dynamic, 1)
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < img.width; i++) {
                for (int j = 0; j < img.height; j++) {
                    // (u, v) の計算
                    // 乱数を使用してピクセル内の位置をずらす
                    double u = (2.0*(i + rnd()) - img.width)/img.width;
                    double v = (2.0*(j + rnd()) - img.height)/img.height;

                    // レイを生成する
                    Ray ray = cam.getRay(-u, -v);

                    // col にはデフォルトで(0, 0, 0) = 黒が入る
                    Vec3 col;
                    Hit res;

                    if(aggregate.intersect(ray, res)) {
                        Ray shadowRay(res.hitPos + 0.001*res.hitNormal, sunDir);
                        Hit res_temp;

                        // 光源の方向へ飛ばしたレイが衝突しなかったら光の強さを計算
                        if(!aggregate.intersect(shadowRay, res_temp)) {
                            col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
                        }
                    }

                    // ピクセルに色を追加する
                    img.addPixel(i, j, col);
                }
            }
        }
    // 平均をとる
    img.divide(N);

    //PPM 出力
    img.ppm_output("super_sampling_parallel.ppm");

    return 0;
}