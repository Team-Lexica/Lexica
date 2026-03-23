#include "Public/fem.h"
#include "Public/grid.h"
#include <iostream>
#include <algorithm>

void printSlice(const std::vector<float>& field, int N, int z, float maxval)
{
    for(int y = 0; y < N; y++){
        for(int x = 0; x < N; x++){
            float v = field[x + N*y + N*N*z] / (maxval + 1e-10f);
            if      (v < 0.01f) std::cout << "  ";
            else if (v < 0.25f) std::cout << ". ";
            else if (v < 0.50f) std::cout << "o ";
            else if (v < 0.75f) std::cout << "O ";
            else                std::cout << "# ";
        }
        std::cout << "\n";
    }
}

int main(){
    int   N          = 40;
    float voxel_size = 0.5f;
    int   iterations = 20;
    float step       = 0.2f;
    float vol_target = 0.4f;

    Grid G(N, voxel_size);
    Sphere S(N/2);
    G.sample(S);
    G.exportf("initial.obj");
    // normalize SDF to [-1, 1]
    float abs_max = 0.0f;
    for(float v : G.model) abs_max = std::max(abs_max, std::abs(v));
    for(float& v : G.model) v /= abs_max;

    G.sampleDensity(0.5f * voxel_size);

    // debug initial state
    float min_m = G.model[0], max_m = G.model[0];
    int neg = 0;
    for(int i = 0; i < N*N*N; i++){
        min_m = std::min(min_m, G.model[i]);
        max_m = std::max(max_m, G.model[i]);
        if(G.model[i] < 0) neg++;
    }
    std::cout << "Initial model range: [" << min_m << ", " << max_m << "]\n";
    std::cout << "Initial negative (solid) voxels: " << neg << "\n\n";

    for(int iter = 0; iter < iterations; iter++)
    {
        // FEM
        SpMat K = assembleK(G.rho, N, 1.0, 0.3, 3.0);
        VecXd F = applyBoundaryConditionsBridge(K, N, voxel_size);
        VecXd u = solve(K, F);

        // stress — compute then smooth
        std::vector<float> stress;
        computeStress(u, G.rho, N, 1.0, 0.3, stress);
        stress = smoothField(stress, N, 1);

        float max_s = *std::max_element(stress.begin(), stress.end());
        float vol   = 0.0f;
        for(float r : G.rho) vol += r;
        vol /= (float)(N*N*N);

        std::cout << "Iter " << iter+1
                  << " | max_stress=" << max_s
                  << " | volume="     << vol << "\n";

        // print shape every 10 iterations
        if((iter+1) % 5 == 0){
            std::cout << "\nShape (mid z-slice, iter " << iter+1 << "):\n";
            printSlice(G.rho, N, N/2, 1.0f);
            std::cout << "\n";
            G.exportf("iter_" + std::to_string(iter+1) + ".obj");
        }

        // update SDF
        levelSetUpdate(G.model, stress, G.rho, N, step, vol_target);
        G.sampleDensity(0.5f * voxel_size);

        // solid count
        int solid = 0;
        for(float r : G.rho) if(r > 0.5f) solid++;
        std::cout << "  solid voxels: " << solid
                  << " | volume: " << vol << "\n\n";
    }

    G.exportf("final.obj");
    std::cout << "Done. Open final.obj\n";
}