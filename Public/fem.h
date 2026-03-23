#pragma once
#include <array>
#include <cmath>
#include <Eigen/Sparse>
#include <vector>
#include <iostream>
#include <algorithm>



// Element stiffness matrix for a single unit voxel
// 8 nodes x 3 DOFs = 24 DOFs per element
// Returns a 24x24 matrix stored as flat array [row*24 + col]

using Ke_t = std::array<double, 24*24>;
using SpMat = Eigen::SparseMatrix<double>;
using Triplet = Eigen::Triplet<double>;

inline Ke_t computeElementStiffness(double E, double nu)
{
    Ke_t Ke{};  // zero initialize

    // Lame constants
    double lambda = E * nu / ((1.0 + nu) * (1.0 - 2.0*nu));
    double mu     = E / (2.0 * (1.0 + nu));

    // 2x2x2 Gauss quadrature points and weights
    // for a unit cube [-0.5, 0.5]^3
    const double gp = 1.0 / std::sqrt(3.0);
    const double gauss[2] = {-gp, gp};
    const double w = 1.0;  // all weights are 1 for 2-point Gauss

    for(int gi = 0; gi < 2; gi++)
    for(int gj = 0; gj < 2; gj++)
    for(int gk = 0; gk < 2; gk++)
    {
        double xi  = gauss[gi];
        double eta = gauss[gj];
        double zeta= gauss[gk];

        // Shape function derivatives w.r.t. reference coords
        // Node ordering: 0=(0,0,0) 1=(1,0,0) 2=(1,1,0) 3=(0,1,0)
        //                4=(0,0,1) 5=(1,0,1) 6=(1,1,1) 7=(0,1,1)
        // Shifted to [-0.5,0.5]: xi in [-0.5,0.5]

        double xip  = 1.0 + 2.0*xi;
        double xim  = 1.0 - 2.0*xi;
        double etap = 1.0 + 2.0*eta;
        double etam = 1.0 - 2.0*eta;
        double zetap= 1.0 + 2.0*zeta;
        double zetam= 1.0 - 2.0*zeta;

        // dN/dxi, dN/deta, dN/dzeta for each of 8 nodes
        double dNdxi[8], dNdeta[8], dNdzeta[8];

        dNdxi[0] = -0.125 * etam * zetam;
        dNdxi[1] =  0.125 * etam * zetam;
        dNdxi[2] =  0.125 * etap * zetam;
        dNdxi[3] = -0.125 * etap * zetam;
        dNdxi[4] = -0.125 * etam * zetap;
        dNdxi[5] =  0.125 * etam * zetap;
        dNdxi[6] =  0.125 * etap * zetap;
        dNdxi[7] = -0.125 * etap * zetap;

        dNdeta[0] = -0.125 * xim * zetam;
        dNdeta[1] = -0.125 * xip * zetam;
        dNdeta[2] =  0.125 * xip * zetam;
        dNdeta[3] =  0.125 * xim * zetam;
        dNdeta[4] = -0.125 * xim * zetap;
        dNdeta[5] = -0.125 * xip * zetap;
        dNdeta[6] =  0.125 * xip * zetap;
        dNdeta[7] =  0.125 * xim * zetap;

        dNdzeta[0] = -0.125 * xim * etam;
        dNdzeta[1] = -0.125 * xip * etam;
        dNdzeta[2] = -0.125 * xip * etap;
        dNdzeta[3] = -0.125 * xim * etap;
        dNdzeta[4] =  0.125 * xim * etam;
        dNdzeta[5] =  0.125 * xip * etam;
        dNdzeta[6] =  0.125 * xip * etap;
        dNdzeta[7] =  0.125 * xim * etap;

        // For a unit cube, Jacobian = 0.5*I, detJ = 0.125
        // dN/dx = 2*dN/dxi  (J^-1 = 2*I)
        double detJ = 0.125;

        // B matrix: 6x24
        // strain = [exx, eyy, ezz, exy, eyz, exz]
        double B[6][24] = {};
        for(int n = 0; n < 8; n++)
        {
            double dNdx = 2.0 * dNdxi[n];
            double dNdy = 2.0 * dNdeta[n];
            double dNdz = 2.0 * dNdzeta[n];

            int col = n * 3;
            B[0][col+0] = dNdx;
            B[1][col+1] = dNdy;
            B[2][col+2] = dNdz;
            B[3][col+0] = dNdy;  B[3][col+1] = dNdx;
            B[4][col+1] = dNdz;  B[4][col+2] = dNdy;
            B[5][col+0] = dNdz;  B[5][col+2] = dNdx;
        }

        // D matrix: 6x6 (isotropic linear elastic)
        double D[6][6] = {};
        D[0][0] = lambda + 2*mu;  D[0][1] = lambda;       D[0][2] = lambda;
        D[1][0] = lambda;         D[1][1] = lambda + 2*mu; D[1][2] = lambda;
        D[2][0] = lambda;         D[2][1] = lambda;        D[2][2] = lambda + 2*mu;
        D[3][3] = mu;
        D[4][4] = mu;
        D[5][5] = mu;

        // Ke += detJ * w^3 * B^T * D * B
        // w=1 for all Gauss points so weight = detJ
        double DB[6][24] = {};
        for(int i = 0; i < 6; i++)
        for(int j = 0; j < 24; j++)
        for(int k = 0; k < 6; k++)
            DB[i][j] += D[i][k] * B[k][j];

        for(int i = 0; i < 24; i++)
        for(int j = 0; j < 24; j++)
        for(int k = 0; k < 6; k++)
            Ke[i*24+j] += detJ * B[k][i] * DB[k][j];
    }

    return Ke;
}



// Convert voxel (x,y,z) node corner index to global node id
// N = grid size, nodes go from 0 to N (inclusive) in each axis
inline int nodeId(int x, int y, int z, int N)
{
    return x + (N+1)*y + (N+1)*(N+1)*z;
}

// Get the 24 DOF indices for a voxel at (vx, vy, vz)
// 8 nodes x 3 DOFs each
inline std::array<int,24> elementDOFs(int vx, int vy, int vz, int N)
{
    // 8 corner nodes of this voxel
    int nodes[8] = {
        nodeId(vx,   vy,   vz,   N),
        nodeId(vx+1, vy,   vz,   N),
        nodeId(vx+1, vy+1, vz,   N),
        nodeId(vx,   vy+1, vz,   N),
        nodeId(vx,   vy,   vz+1, N),
        nodeId(vx+1, vy,   vz+1, N),
        nodeId(vx+1, vy+1, vz+1, N),
        nodeId(vx,   vy+1, vz+1, N)
    };

    std::array<int,24> dofs;
    for(int i = 0; i < 8; i++){
        dofs[i*3+0] = nodes[i]*3;      // x displacement
        dofs[i*3+1] = nodes[i]*3 + 1;  // y displacement
        dofs[i*3+2] = nodes[i]*3 + 2;  // z displacement
    }
    return dofs;
}

// Assemble global stiffness matrix from density field
// rho: flat array of size N^3, indexed [x + N*y + N*N*z]
// E0: Young's modulus of solid material
// nu: Poisson's ratio
// penal: SIMP penalization (rho^penal scales stiffness)
inline SpMat assembleK(
    const std::vector<float>& rho,
    int N,
    double E0,
    double nu,
    double penal)
{
    int ndof = (N+1)*(N+1)*(N+1)*3;
    Ke_t Ke0 = computeElementStiffness(E0, nu);

    std::vector<Triplet> triplets;
    // each voxel contributes 24x24 = 576 entries
    triplets.reserve(N*N*N * 576);

    for(int vz = 0; vz < N; vz++)
    for(int vy = 0; vy < N; vy++)
    for(int vx = 0; vx < N; vx++)
    {
        int idx = vx + N*vy + N*N*vz;
        double r = rho[idx];

        // SIMP: scale stiffness by rho^penal
        // E_min prevents singular K in void regions
        double E_min = 1e-3 * E0;
        double E_eff = E_min + std::pow(r, penal) * (E0 - E_min);
        double scale = E_eff / E0;

        auto dofs = elementDOFs(vx, vy, vz, N);

        for(int i = 0; i < 24; i++)
        for(int j = 0; j < 24; j++)
            triplets.push_back({dofs[i], dofs[j], scale * Ke0[i*24+j]});
    }

    SpMat K(ndof, ndof);
    K.setFromTriplets(triplets.begin(), triplets.end());
    // setFromTriplets automatically sums duplicate entries
    return K;
}

#include <Eigen/Dense>

using VecXd = Eigen::VectorXd;

// Apply cantilever boundary conditions
// Fixed: left face (vx=0), all DOFs clamped to zero
// Modifies K in place, returns force vector F
inline VecXd applyBoundaryConditions(SpMat& K, int N, float voxel_size)
{
    int ndof = (N+1)*(N+1)*(N+1)*3;
    VecXd F = VecXd::Zero(ndof);

    // collect fixed DOFs — left face (x=0)
    std::vector<bool> is_fixed(ndof, false);
    for(int vz = 0; vz <= N; vz++)
    for(int vy = 0; vy <= N; vy++)
    {
        int nid = nodeId(0, vy, vz, N);
        is_fixed[nid*3+0] = true;
        is_fixed[nid*3+1] = true;
        is_fixed[nid*3+2] = true;
    }

    // rebuild K with fixed DOFs zeroed out
    std::vector<Triplet> triplets;
    for(int k = 0; k < K.outerSize(); k++)
    for(Eigen::InnerIterator it(K, k); it; ++it)
    {
        int r = it.row();
        int c = it.col();
        if(is_fixed[r] || is_fixed[c])
        {
            if(r == c) triplets.push_back({r, c, 1.0});
            // off-diagonal fixed entries just get dropped (zero)
        }
        else
        {
            triplets.push_back({r, c, it.value()});
        }
    }

    K.setFromTriplets(triplets.begin(), triplets.end());

    // zero F at fixed DOFs (already zero from initialization)
    // apply downward force on right face (x=N)
    int right_face_nodes = (N+1)*(N+1);
    double force_per_node = -1.0 / right_face_nodes;

    for(int vz = 0; vz <= N; vz++)
    for(int vy = 0; vy <= N; vy++)
    {
        int nid = nodeId(N, vy, vz, N);
        int dof_y = nid*3 + 1;
        if(!is_fixed[dof_y])
            F[dof_y] = force_per_node;
    }

    return F;
}

inline VecXd solve(SpMat& K, const VecXd& F)
{
    Eigen::ConjugateGradient<SpMat, Eigen::Lower|Eigen::Upper> cg;
    cg.setMaxIterations(2000);
    cg.setTolerance(1e-5);
    cg.compute(K);
    VecXd u = cg.solve(F);

    std::cout << "CG iterations: " << cg.iterations() << "\n";
    std::cout << "CG residual:   " << cg.error()      << "\n";

    return u;
}

inline void computeStress(
    const VecXd& u,
    const std::vector<float>& rho,
    int N,
    double E0,
    double nu,
    std::vector<float>& stress)  // output: von Mises stress per voxel
{
    stress.resize(N*N*N, 0.0f);

    double lambda = E0 * nu / ((1.0 + nu) * (1.0 - 2.0*nu));
    double mu     = E0 / (2.0 * (1.0 + nu));

    // D matrix
    double D[6][6] = {};
    D[0][0] = lambda+2*mu; D[0][1] = lambda;       D[0][2] = lambda;
    D[1][0] = lambda;      D[1][1] = lambda+2*mu;  D[1][2] = lambda;
    D[2][0] = lambda;      D[2][1] = lambda;        D[2][2] = lambda+2*mu;
    D[3][3] = mu;
    D[4][4] = mu;
    D[5][5] = mu;

    // evaluate at element centroid (xi=eta=zeta=0)
    // at centroid all shape function derivatives simplify
    double dNdx[8] = {-0.25, 0.25, 0.25, -0.25, -0.25,  0.25,  0.25, -0.25};
    double dNdy[8] = {-0.25,-0.25, 0.25,  0.25, -0.25, -0.25,  0.25,  0.25};
    double dNdz[8] = {-0.25,-0.25,-0.25, -0.25,  0.25,  0.25,  0.25,  0.25};

    // B matrix at centroid: 6x24
    double B[6][24] = {};
    for(int n = 0; n < 8; n++)
    {
        int col = n*3;
        B[0][col+0] = dNdx[n];
        B[1][col+1] = dNdy[n];
        B[2][col+2] = dNdz[n];
        B[3][col+0] = dNdy[n];  B[3][col+1] = dNdx[n];
        B[4][col+1] = dNdz[n];  B[4][col+2] = dNdy[n];
        B[5][col+0] = dNdz[n];  B[5][col+2] = dNdx[n];
    }

    for(int vz = 0; vz < N; vz++)
    for(int vy = 0; vy < N; vy++)
    for(int vx = 0; vx < N; vx++)
    {
        int idx = vx + N*vy + N*N*vz;
        if(rho[idx] < 0.1f) continue;  // skip void

        auto dofs = elementDOFs(vx, vy, vz, N);

        // extract element displacement vector ue (24 values)
        double ue[24];
        for(int i = 0; i < 24; i++)
            ue[i] = u[dofs[i]];

        // strain = B * ue
        double strain[6] = {};
        for(int i = 0; i < 6; i++)
        for(int j = 0; j < 24; j++)
            strain[i] += B[i][j] * ue[j];

        // stress = D * strain
        double sig[6] = {};
        for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++)
            sig[i] += D[i][j] * strain[j];

        // von Mises stress
        double sxx = sig[0], syy = sig[1], szz = sig[2];
        double sxy = sig[3], syz = sig[4], sxz = sig[5];

        double vm = std::sqrt(0.5 * (
            (sxx-syy)*(sxx-syy) +
            (syy-szz)*(syy-szz) +
            (szz-sxx)*(szz-sxx) +
            6.0*(sxy*sxy + syz*syz + sxz*sxz)
        ));

        stress[idx] = (float)vm;
    }
}


inline void levelSetUpdate(
    std::vector<float>& model,
    const std::vector<float>& stress,
    const std::vector<float>& rho,
    int N,
    float step,
    float vol_target)
{
    int total = N*N*N;

    // mean stress over solid voxels
    double mean_stress = 0.0;
    int solid_count = 0;
    for(int i = 0; i < total; i++){
        if(rho[i] > 0.1f){
            mean_stress += stress[i];
            solid_count++;
        }
    }
    if(solid_count == 0) return;
    mean_stress /= solid_count;

    // current volume fraction
    double vol = 0.0;
    for(int i = 0; i < total; i++) vol += rho[i];
    vol /= total;

    double vol_error = vol - vol_target;
    double threshold = mean_stress * (1.0 + vol_error * 3.0);
    threshold = std::max(threshold, mean_stress * 0.1);
    threshold = std::min(threshold, mean_stress * 3.0);

    std::cout << "  [update] vol=" << vol
              << " vol_error=" << vol_error
              << " mean_s=" << mean_stress
              << " threshold=" << threshold << "\n";

    // removal rate — how many solid voxels to remove this iteration
    double removal_rate = 0.08;
    if(vol_error > 0.3)      removal_rate = 0.15;
    else if(vol_error > 0.1) removal_rate = 0.10;

    // find stress cutoff by sorting solid stresses
    std::vector<float> solid_stresses;
    solid_stresses.reserve(solid_count);
    for(int i = 0; i < total; i++)
        if(rho[i] > 0.1f) solid_stresses.push_back(stress[i]);

    std::sort(solid_stresses.begin(), solid_stresses.end());
    int cutoff_idx = (int)(removal_rate * solid_stresses.size());
    float removal_cutoff = solid_stresses[std::max(0, cutoff_idx - 1)];

    // effective cutoff: must satisfy both the rate limit and threshold
    float effective_cutoff = std::min(removal_cutoff, (float)threshold);

    std::cout << "  [update] removal_rate=" << removal_rate
              << " cutoff=" << effective_cutoff << "\n";

    int removed = 0, reinforced = 0, grown = 0;
    for(int i = 0; i < total; i++)
    {
        if(rho[i] > 0.1f)
        {
            if(stress[i] < effective_cutoff){
                model[i] += step;   // low stress — remove
                removed++;
            } else {
                
                reinforced++;
            }
        }
        else
        {
            // void near surface grows back if volume below target
            if(model[i] > -0.3f && vol < vol_target){
                model[i] -= step * 0.3f;
                grown++;
            }
        }
    }

    std::cout << "  [update] removed=" << removed
              << " reinforced=" << reinforced
              << " grown=" << grown << "\n";
}



inline std::vector<float> smoothField(
    const std::vector<float>& field, int N, int radius = 1)
{
    std::vector<float> out(N*N*N, 0.0f);
    for(int z = 0; z < N; z++)
    for(int y = 0; y < N; y++)
    for(int x = 0; x < N; x++)
    {
        float sum = 0.0f;
        int   cnt = 0;
        for(int dz = -radius; dz <= radius; dz++)
        for(int dy = -radius; dy <= radius; dy++)
        for(int dx = -radius; dx <= radius; dx++)
        {
            int nx = x+dx, ny = y+dy, nz = z+dz;
            if(nx<0||nx>=N||ny<0||ny>=N||nz<0||nz>=N) continue;
            sum += field[nx + N*ny + N*N*nz];
            cnt++;
        }
        out[x + N*y + N*N*z] = sum / cnt;
    }
    return out;
}


inline VecXd applyBoundaryConditionsBridge(SpMat& K, int N, float voxel_size)
{
    int ndof = (N+1)*(N+1)*(N+1)*3;
    VecXd F = VecXd::Zero(ndof);

    // collect fixed DOFs
    // bottom-left column (x=0, y=0) — pin: fix x and y
    // bottom-right column (x=N, y=0) — pin: fix x and y
    std::vector<bool> is_fixed(ndof, false);

    for(int vz = 0; vz <= N; vz++)
    {
        // bottom-left corner column
        int nid_bl = nodeId(0, 0, vz, N);
        is_fixed[nid_bl*3+0] = true;
        is_fixed[nid_bl*3+1] = true;
        is_fixed[nid_bl*3+2] = true;

        // bottom-right corner column
        int nid_br = nodeId(N, 0, vz, N);
        is_fixed[nid_br*3+0] = true;
        is_fixed[nid_br*3+1] = true;
        is_fixed[nid_br*3+2] = true;
    }

    // rebuild K with BCs applied
    std::vector<Triplet> triplets;
    for(int k = 0; k < K.outerSize(); k++)
    for(Eigen::InnerIterator it(K, k); it; ++it)
    {
        int r = it.row(), c = it.col();
        if(is_fixed[r] || is_fixed[c])
        {
            if(r == c) triplets.push_back({r, c, 1.0});
        }
        else
            triplets.push_back({r, c, it.value()});
    }
    K.setFromTriplets(triplets.begin(), triplets.end());

    // distributed downward load on entire top face (y=N)
    int top_face_nodes = (N+1)*(N+1);
    double force_per_node = -1.0 / top_face_nodes;

    for(int vz = 0; vz <= N; vz++)
    for(int vx = 0; vx <= N; vx++)
    {
        int nid = nodeId(vx, N, vz, N);
        int dof_y = nid*3 + 1;
        if(!is_fixed[dof_y])
            F[dof_y] = force_per_node;
    }

    return F;
}