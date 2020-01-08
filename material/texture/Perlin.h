#ifndef RAYTRACING_PERLIN_H
#define RAYTRACING_PERLIN_H
#include "../../utility/Vec3.h"
#include "../../utility/util.h"
#include <vector>

// Encapsulates a pseudorandom gradient noise developed by Ken Perlin.
// This particular implemntation uses 3-dimensional grid values.
// The process of pseudorandom generations involves typically
// three steps:
// 1. Defining a grid with random gradient vectors
// 2. Computing the dot product between the distance-gradient vectors
// 3. Interpolating between these values.
// Also added is a final step to produce turbulence, which gives
// the Perlin Noise a look similar to marble.
class Perlin {
public:
    Perlin(int num_permutations) : num_permutations_{num_permutations} {
        random_vectors_.resize(num_permutations_);
        random_vectors_ = perlin_generate();
    }

    // A hashing which produces the Perlin noise.
    // While there are many ways to go about this
    // function, this follows closely the method used
    // by raytracing.github.io and Andrew Kensler.
    value_type noise(const BoundVec3& p) const {
        const value_type u = p.x() - std::floor(p.x());
        const value_type v = p.y() - std::floor(p.y());
        const value_type w = p.z() - std::floor(p.z());
        const int i = std::floor(p.x());
        const int j = std::floor(p.y());
        const int k = std::floor(p.z());

        FreeVec3 c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = random_vectors_[
                            perm_x_[(i+di) & 255] ^
                            perm_y_[(j+dj) & 255] ^
                            perm_z_[(k+dk) & 255]
                            ];
                }
            }
        }
        return perlin_interpolation(c, u, v, w);
    }

    // Interpolates between the grid point coordinates.
    value_type perlin_interpolation(FreeVec3 c[2][2][2], value_type u, value_type v, value_type w) const {
        // Hermite cubic to round off the interpolation.
        const value_type uu = u * u * (3 - 2 * u);
        const value_type vv = v * v * (3 - 2 * v);
        const value_type ww = w * w * (3 - 2 * w);
        value_type accumulator = 0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    FreeVec3 weight_v(u-i, v-j, w-k);
                    accumulator +=
                            (i * uu + (1 - i) * (1 - uu)) *
                            (j * vv + (1 - j) * (1 - vv)) *
                            (k * ww + (1 - k) * (1 - ww)) *
                            (weight_v.dot(c[i][j][k]));
                }
            }
        }
        return accumulator;
    }

    // Produces random unit vectors in a std::vector of size
    // num_permutations_.
    std::vector<FreeVec3> perlin_generate() {
        std::vector<FreeVec3> p(num_permutations_);
        for (int i = 0; i < p.size(); ++i) {
            const value_type x_random = 2 * random_value() - 1;
            const value_type y_random = 2 * random_value() - 1;
            const value_type z_random = 2 * random_value() - 1;
            p[i] = UnitVec3(x_random, y_random, z_random).to_free();
        }
        return p;
    }

    // Permutes the current std::vector p by replacing
    // the current index's value with a randomly selected
    // index's value.
    void permute(std::vector<int> p) {
        for (int i = p.size() - 1; i > 0; --i) {
            const int target = int(random_value() * i+1);
            int temp = p[i];
            p[i] = p[target];
            p[target] = temp;
        }
    }

    // Produces an std::vector that is first numbered
    // from [0, num_permutations_], and then permuted.
    std::vector<int> perlin_generate_permutation() {
        std::vector<int> p(num_permutations_);
        std::iota(p.begin(), p.end(), 0);
        permute(p);
        return p;
    }

    // Multiple summed frequencies, or the sum of repeated calls to noise().
    // The number of calls is determined by 'turbulence_depth'.
    value_type turbulence(const BoundVec3& p, int turbulence_depth) const {
        value_type accumulator = 0;
        value_type weight = 1.0;
        BoundVec3 temporary_p = p;
        for (int i = 0; i < turbulence_depth; ++i) {
            accumulator += weight * noise(temporary_p);
            weight *= 0.5;
            const FreeVec3 v = FreeVec3(temporary_p) * 2.0;
            temporary_p = BoundVec3(v);
        }
        return std::fabs(accumulator);
    }
private:
    // The number of permutations to be generated.
    const int num_permutations_;
    std::vector<FreeVec3> random_vectors_;
    const std::vector<int> perm_x_ = perlin_generate_permutation();
    const std::vector<int> perm_y_ = perlin_generate_permutation();
    const std::vector<int> perm_z_ = perlin_generate_permutation();
};

#endif //RAYTRACING_PERLIN_H
