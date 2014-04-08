#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <exception>

#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <Accelerate/Accelerate.h>

#include "constraintsolver2d.h"
#include "constraintsolver3d.h"
#include "Graph.h"

using namespace std;
using namespace agi3d;

Graph::Graph() : _isLoaded(false), _name("unknown"), _dataDir(""){
  
}

Graph::~Graph() {
  
}

//FIXME:ファイルを開く度にモデルを使いまわすのを止めたい
//      ファイルを開いたら、古いモデルはdestroyして、新しいモデルをnewする
void Graph::reset() {
  if(isNeighbor) {
    //delete[] isNeighbor;
  }
  if(isdrawingNodes) {
    //delete[] isdrawingNodes;
  }
  if(edgeAttribute) {
    //delete[] edgeAttribute;
  }
  if(isdrawingEdges) {
    //delete[] isdrawingEdges;
  }
  
  isNeighbor = new bool[N];
  isdrawingNodes = new bool[N];
  
  edgeAttribute = new bool[M];
  isdrawingEdges = new bool[M];
  
  for (int i = 0; i < N; i++) {
    isNeighbor[i] = false;
    isdrawingNodes[i] = true;
  }
  
  for (int i = 0; i < M; i++) {
    edgeAttribute[i] = true;
    isdrawingEdges[i] = true;
  }
}

void Graph::changeProjectionFactor(float f, E_Layout layout) {
  if(!this->isLoaded()) { return; }
  switch (layout) {
    case E_Layout::D2:
      this->updateProjection2D(f);
      this->notify(E_ObserveType::NeedRelayout);
      break;
    case E_Layout::D3:
      this->updateProjection3D(f);
      this->notify(E_ObserveType::NeedRelayout);
      break;
    default:
      throw invalid_argument("unkown layout type found.");
  }
}

void Graph::changeScaleLayout(float f, E_Layout layoutType) {
  switch (layoutType) {
    case E_Layout::D2:
    {
      this->updateScale2D(f);
      this->notify(E_ObserveType::NeedRelayout);
      break;
    }
    case E_Layout::D3:
    {
      this->updateScale3D(f);
      this->notify(E_ObserveType::NeedRelayout);
      break;
    }
    default: {
      break;
    }
  }
}

void Graph::changeNodeThreshold(float b, float t) {
  for (int i = 0; i < N; i++) {
    isdrawingNodes[i] = ((nodevalues[i] >= b) && (nodevalues[i] <= t));
  }
  for (int i = 0; i < N; i++) {
    if (isdrawingNodes[i]) {
      bool f = false;
      for (size_t j = 0; j < neighbor[i].size(); j++) {
        f |= isdrawingNodes[neighbor[i][j]];
        if (f) break;
      }
      if (!f) isdrawingNodes[i] = false;
    }
  }
  this->notify(E_ObserveType::RefreshOnly);
}

void Graph::changeEdgeThreshold(float b, float t) {
  for (int i = 0; i < M; i++) {
    isdrawingEdges[i] = ((edgevalues[i] >= b) && (edgevalues[i] <= t));
  }
  this->notify(E_ObserveType::RefreshOnly);
}

bool Graph::loadData(const std::string &filePath){
  string fname = string(filePath);
  int path_i = fname.find_last_of("/");
  int ext_i = fname.find_last_of(".");
  string f_ext = fname.substr(ext_i + 1);
  string filename = fname.substr(path_i + 1);
  _dataDir = fname.substr(0, path_i + 1);
  if (f_ext == "txt") {
    if (filename.find("DisMat") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 11);
      loadMatrixData_t(filePath.c_str());
    } else if (filename.find("Data.") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 9);
      loadLayoutData_t(filePath.c_str());
    } else if (filename.find("DataAll") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 12);
      loadLayoutData_t(filePath.c_str());
    } else {
      cerr << "This file is not available" << endl;
      return false;
    }
  } else if (f_ext == "bin") {
    if (filename.find("DisMat") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 11);
      loadMatrixData_b(filePath.c_str());
    } else if (filename.find("Data.") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 15);
      loadLayoutData_b(filePath.c_str());
    } else if (filename.find("DataAll") != string::npos) {
      _name = fname.substr(path_i + 1, fname.size() - path_i - 18);
      loadLayoutData_b(filePath.c_str());
    } else {
      cerr << "This file is not available" << endl;
      return false;
    }
  }
  _isLoaded = true;
  return true;
}

//Params about Graph

double Graph::getETime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double) tv.tv_usec * 1e-6;
}

int Graph::strToInt(const string &str) {
  int t;
  stringstream ss;
  ss << str;
  ss >> t;
  return t;
}

string Graph::IntToString(int num) {
  stringstream ss;
  ss << num;
  return ss.str();
}

void Graph::resetLayout3D() {
  scale = 1.0f;
  delta = 0.5;
  
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < 3; j++) {
      E_3D[i + j * dim] = E_3D_init[i + j * dim];
    }
  }
  
  delete[] Layout3D;
  Layout3D = new float[N * 3];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 3, dim, scale, P, N, E_3D, dim, 0.0, Layout3D, N);
  
  {
    delete nodevalues;
    nodevalues = new float[N];
    for (int i = 0; i < N; i++) {
      nodevalues[i] = 1.0f;
    }
    nodevalue_max = 5;
    nodevalue_min = 0;
    
    delete edgevalues;
    edgevalues = new float[M];
    for (int i = 0; i < M; i++) {
      edgevalues[i] = 1.0f;
    }
    edgevalue_max = 1;
    edgevalue_min = 0;
  }
}

void Graph::resetLayout2D() {
  scale = 1.0f;
  delta = 0.5;
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < dim; j++) {
      E_2D[j + i * dim] = E_2D_init[j + i * dim];
    }
  }
  
  delete[] Layout2D;
  Layout2D = new float[N * 2];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 2, dim, scale, P, N, E_2D, dim, 0.0, Layout2D, N);
  
  {
    delete nodevalues;
    nodevalues = new float[N];
    for (int i = 0; i < N; i++) {
      nodevalues[i] = 1.0f;
    }
    nodevalue_max = 5;
    nodevalue_min = 0;
    
    delete edgevalues;
    edgevalues = new float[M];
    for (int i = 0; i < M; i++) {
      edgevalues[i] = 1.0f;
    }
    edgevalue_max = 1;
    edgevalue_min = 0;
  }
}

void Graph::calcmdsLayout() {
  if(this->isLoaded()) {
    delete[] lambdas;
    delete[] P_norms;
    delete[] P;
  }
  
  float * D2 = new float[N * N];
  for (int i = 0; i < N; i++) {
    D2[i + i * N] = 0.0f;
    for (int j = i + 1; j < N; j++) {
      float d2 = (float) (D[i][j] * D[i][j]);
      D2[i + j * N] = d2;
      D2[j + i * N] = d2;
    }
  }
  
  float * H = new float[N * N];
  for (int i = 0; i < N; i++) {
    H[i + N * i] = 1.0f - (float) 1.0 / N;
    for (int j = i + 1; j < N; j++) {
      H[i + N * j] = -(float) 1.0 / N;
      H[j + N * i] = -(float) 1.0 / N;
    }
  }
  
  float * b = new float[N * N];
  float * B = new float[N * N];
  float alpha = 1.0, beta = 0.0f;
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, N, N, alpha, H, N, D2, N, beta, b, N);
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, N, N, alpha, b, N, H, N, beta, B, N);
  
  //Float
  int lwork, info;
  float *A = new float[N * N];
  float *w = new float[N];
  
  for (int i = 0; i < N; i++) {
    A[i + i * N] = -0.5f * B[i + i * N];
    for (int j = i + 1; j < N; j++) {
      A[i + j * N] = -0.5f * B[i + j * N];
      A[j + i * N] = -0.5f * B[j + i * N];
    }
  }
  
  lwork = -1;
  float *work = new float[1];
  char jobz = 'V', uplo = 'U';
  ssyev_(&jobz, &uplo, &N, A, &N, w, work, &lwork, &info);
  lwork = (int) work[0];
  delete[]work;
  work = new float[max((int) 1, lwork)];
  ssyev_(&jobz, &uplo, &N, A, &N, w, work, &lwork, &info);
  
  dim = 0;
  float eps = 0.01;
  for (int i = N - 1; i >= 0; i--) {
    if (w[i] > eps) dim++;
    else break;
  }
  
  if (dim > maxDimension) dim = maxDimension;
  
  lambdas = new float[dim];
  for (int i = 0; i < dim; i++) {
    lambdas[i] = w[N - 1 - i];
  }
  
  float * L = new float[N * dim];
  
  P = new float[N * dim];
  P_norms = new float[N];
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < dim; j++) {
      if (i == j) L[i + j * N] = (float) sqrt(w[N - 1 - j]);
      else L[i + j * N] = 0.0f;
      P[i + j * N] = 0.0f;
    }
  }
  
  float * U = new float[N * N];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      U[i + j * N] = A[j + (N - 1 - i) * N];
    }
  }
  
  //High Dimensional Layout : P
  cblas_sgemm(CblasColMajor, CblasTrans, CblasNoTrans,
              N, dim, N, alpha, U, N, L, N, beta, P, N);
  
  float pij = 0;
  for (int i = 0; i < N; i++) {
    P_norms[i] = 0.0f;
    for (int j = 0; j < dim; j++) {
      pij = P[i + j * N];
      P_norms[i] += pij*pij;
    }
  }
  
  delete work;
  delete w;
  delete A;
  delete D2;
  delete H;
  delete B;
  delete b;
  delete L;
  delete U;
}

void Graph::updateScale3D(float r) {
  scale = r;
  delete[] Layout3D;
  Layout3D = new float[N * 3];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 3, dim, scale, P, N, E_3D, dim, 0.0, Layout3D, N);
}

void Graph::updateScale2D(float r) {
  scale = r;
  delete[] Layout2D;
  Layout2D = new float[N * 2];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 2, dim, scale, P, N, E_2D, dim, 0.0, Layout2D, N);
}

void Graph::updateProjection3D(float r) {
  delta = r;
  fvector e_3d[3];
  for (int i = 0; i < 3; i++) {
    e_3d[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 3 == i) {
        e_3d[i][j] = pow((double) lambdas[j], (double) delta);
      } else e_3d[i][j] = 0.0f;
    }
    e_3d[i] = e_3d[i] / norm_2(e_3d[i]);
  }
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < dim; j++) {
      E_3D[j + i * dim] = e_3d[i][j];
    }
  }
  
  delete[] Layout3D;
  Layout3D = new float[N * 3];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 3, dim, scale, P, N, E_3D, dim, 0.0, Layout3D, N);
}

void Graph::updateProjection2D(float r) {
  delta = r;
  fvector e_2d[2];
  for (int i = 0; i < 2; i++) {
    e_2d[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 2 == i) e_2d[i][j] = pow((double) lambdas[j], (double) delta);
      else e_2d[i][j] = 0.0f;
    }
    e_2d[i] = e_2d[i] / norm_2(e_2d[i]);
  }
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < dim; j++) {
      E_2D[j + i * dim] = e_2d[i][j];
    }
  }
  
  delete[] Layout2D;
  Layout2D = new float[N * 2];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 2, dim, scale, P, N, E_2D, dim, 0.0, Layout2D, N);
}

void Graph::updateDimension3D(float r) {
  int _dim = (int) (r * dim);
  if (_dim < 3) _dim = 3;
  
  fvector e_3d[3];
  for (int i = 0; i < 3; i++) {
    e_3d[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 3 == i && j < _dim) {
        e_3d[i][j] = pow((double) lambdas[j], (double) delta);
      } else e_3d[i][j] = 0.0f;
    }
    e_3d[i] = e_3d[i] / norm_2(e_3d[i]);
  }
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < dim; j++) {
      E_3D[j + i * dim] = e_3d[i][j];
      E_3D_init[j + i * dim] = e_3d[i][j];
    }
  }
  
  delete[] Layout3D;
  Layout3D = new float[N * 3];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 3, dim, scale, P, N, E_3D, dim, 0.0, Layout3D, N);
}

void Graph::updateDimension2D(float r) {
  int _dim = (int) (r * dim);
  if (_dim < 2) _dim = 2;
  
  fvector e_2d[2];
  for (int i = 0; i < 2; i++) {
    e_2d[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 2 == i && j < _dim) e_2d[i][j] = pow((double) lambdas[j], (double) delta);
      else e_2d[i][j] = 0.0f;
    }
    e_2d[i] = e_2d[i] / norm_2(e_2d[i]);
  }
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < dim; j++) {
      E_2D[j + i * dim] = e_2d[i][j];
    }
  }
  
  delete[] Layout2D;
  Layout2D = new float[N * 2];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 2, dim, scale, P, N, E_2D, dim, 0.0, Layout2D, N);
}

void Graph::calc3DLayout() {
  fvector e[3];
  for (int i = 0; i < 3; i++) {
    e[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 3 == i) {
        e[i][j] = sqrt(lambdas[j]);
      } else e[i][j] = 0.0f;
    }
    e[i] = e[i] / norm_2(e[i]);
  }
  
  E_3D = new float[dim * 3];
  E_3D_init = new float[dim * 3];
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < dim; j++) {
      E_3D[j + i * dim] = e[i][j];
      E_3D_init[j + i * dim] = e[i][j];
    }
  }
  
  if(this->isLoaded()) {
    delete[] Layout3D;
  }
  Layout3D = new float[N * 3];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 3, dim, 1.0, P, N, E_3D, dim, 0.0, Layout3D, N);
  
  //Set Initial Value of resctriction
  init3D[0] = 0.9;
  init3D[1] = -0.1;
  init3D[2] = 0.1;
  init3D[3] = -0.1;
  init3D[4] = 0.9;
  init3D[5] = 0.1;
  init3D[6] = 0.1;
  init3D[7] = 0.1;
  init3D[8] = 0.9;
  init3D[9] = 0.1;
  init3D[10] = -0.1;
  init3D[11] = 0.1;
  init3D[12] = -0.1;
  init3D[13] = 0.1;
  init3D[14] = 0.1;
}

void Graph::calc2DLayout() {
  fvector e[2];
  for (int i = 0; i < 2; i++) {
    e[i] = fvector(dim);
    for (int j = 0; j < dim; j++) {
      if (j % 2 == i) e[i][j] = sqrt(lambdas[j]);
      else e[i][j] = 0.0f;
    }
    e[i] = e[i] / norm_2(e[i]);
  }
  
  E_2D = new float[dim * 2];
  E_2D_init = new float[dim * 2];
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < dim; j++) {
      E_2D[j + i * dim] = e[i][j];
      E_2D_init[j + i * dim] = e[i][j];
    }
  }
  
  if(this->isLoaded()) {
    delete[] Layout2D;
  }
  Layout2D = new float[N * 2];
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
              N, 2, dim, scale, P, N, E_2D, dim, 0.0, Layout2D, N);
  
  //Set Initial Value of resctriction
  init2D[0] = 0.9;
  init2D[1] = -0.1;
  init2D[2] = 0.1;
  init2D[3] = 0.9;
  init2D[4] = 0.4;
  init2D[5] = 0.4;
}

void Graph::loadNodeAttrData(int n) {
  bool isNormal = false;
  std::string fileName = _dataDir;
  switch (n) {
    case 1: fileName += _name + "degree.txt";
      break;
    case 2: fileName += _name + "clcent.txt";
      break;
    case 3: fileName += _name + "bwcent.txt";
      break;
    case 4: fileName += _name + "clcoeff.txt";
      break;
    case 5: fileName += _name + "evcent.txt";
      break;
    case 6: fileName += _name + "pr.txt";
      break;
    default: isNormal = true;
      break;
  }
  
  if (isNormal) {
    for (int i = 0; i < N; i++) {
      nodevalues[i] = 1.0f;
    }
    nodevalue_max = 5;
    nodevalue_min = 0;
  } else {
    ifstream ifs(fileName.c_str());
    if (!ifs.fail()) {
      nodevalue_max = 0;
      nodevalue_min = 1000000000000;
      for (int i = 0; i < N; i++) {
        ifs >> nodevalues[i];
        nodevalue_min = min(nodevalue_min, nodevalues[i]);
        nodevalue_max = max(nodevalue_max, nodevalues[i]);
      }
    } else {
      if (n == 1) {
        nodevalue_max = 0;
        nodevalue_min = 1000000000000;
        for (int i = 0; i < N; i++) {
          nodevalues[i] = neighbor[i].size();
          nodevalue_min = min(nodevalue_min, nodevalues[i]);
          nodevalue_max = max(nodevalue_max, nodevalues[i]);
        }
      } else {
        cerr << "File not found\n";
      }
    }
    ifs.close();
  }
}

void Graph::loadEdgeAttrData(int n) {
  
  bool isDefault = false;
  std::string fileName = _dataDir;
  switch (n) {
    case 1: fileName += _name + "Simpson.txt";
      break;
    case 2: fileName += _name + "SimpsonEx.txt";
      break;
    case 3: fileName += _name + "ebw.txt";
      break;
    case 4: fileName += _name + "Weight.txt";
      break;
    default: isDefault = true;
      break;
  }
  
  if (isDefault) {
    for (int i = 0; i < M; i++) {
      edgevalues[i] = 1.0;
    }
    edgevalue_min = 0;
    edgevalue_max = 1;
  } else {
    ifstream ifs(fileName.c_str());
    if (!ifs.fail()) {
      edgevalue_max = 0;
      edgevalue_min = 100000000;
      int from, to;
      for (int i = 0; i < M; i++) {
        ifs >> from >> to >> edgevalues[i];
        edgevalue_min = min(edgevalue_min, edgevalues[i]);
        edgevalue_max = max(edgevalue_max, edgevalues[i]);
      }
      ifs.close();
    } else {
      cout << "File not found" << endl;
    }
  }
}

void Graph::calcSimpson()
{
  string filename = _dataDir + _name + "Simpson.txt";
  ofstream ofs(filename.c_str());
  edgevalue_max = 0;
  edgevalue_min = 100000000;
  for (int i = 0; i < M; i++) {
    int from = edges[i].first, to = edges[i].second;
    float inter = 1.0f;
    for (int j = 0; j < N; j++) {
      if (D[from][j] == 1 && D[to][j] == 1) {
        inter += 1.0f;
      }
    }
    float f = (float) min(neighbor[from].size(), neighbor[to].size());
    edgevalues[i] = (float) inter / (float) f;
    edgevalue_min = min(edgevalue_min, edgevalues[i]);
    edgevalue_max = max(edgevalue_max, edgevalues[i]);
    ofs << from << " " << to << " " << edgevalues[i] << endl;
  }
  ofs.close();
}

void Graph::calcSimpsonEx() {
    string filename = _dataDir + _name + "SimpsonEx.txt";
    ofstream ofs(filename.c_str());
    edgevalue_max = 0;
    edgevalue_min = 100000000;
    for (int i = 0; i < M; i++) {
      int from = edges[i].first, to = edges[i].second;
      float count = 0.0f;
      for (int j = 0; j < N; j++) {
        if (D[from][j] == 1 && D[to][j] == 1) {
          count += 1.0f;
        }
      }
      int a = min(neighbor[from].size(), neighbor[to].size());
      if (a >= 2) {
        edgevalues[i] = (float) count / (float) a;
      } else {
        edgevalues[i] = 0;
      }
      edgevalue_min = min(edgevalue_min, edgevalues[i]);
      edgevalue_max = max(edgevalue_max, edgevalues[i]);
      ofs << from << " " << to << " " << edgevalues[i] << endl;
    }
    ofs.close();
}

void Graph::loadLabelData() {
  const std::string& graphName = this->getName();
  string labeldata = "../data/" + graphName + "/" + graphName + "labels.txt";
  ifstream lifs(labeldata.c_str());
  if (lifs.fail()) {
    cout << "No Label Data" << endl;
    for (int i = 0; i < N; i++) {
      labels.push_back(IntToString(i + 1));
    }
  } else {
    string _str = "";
    
    for (int i = 0; i < N; i++) {
      getline(lifs, _str);
      labels.push_back(_str);
    }
  }
  lifs.close();
}

//Set Default Node & Edge value
void Graph::setNodeEdgeValue() {
  if(isLoaded()) {
    delete[] nodevalues;
    delete[] edgevalues;
  }
  nodevalues = new float[N];
  for (int i = 0; i < N; i++) {
    nodevalues[i] = 1.0f;
  }
  nodevalue_max = 5;
  nodevalue_min = 0;
  
  
  edgevalues = new float[M];
  for (int i = 0; i < M; i++) {
    edgevalues[i] = 1.0f;
  }
  edgevalue_max = 1;
  edgevalue_min = 0;
}

void Graph::loadMatrixData_t(const char * data) {
  //Free Memor
  {
    if (memory_status) {
      for (int i = 0; i < N; i++) {
        delete[] D[i];
      }
      delete[] D;
      memory_status = false;
    }
    for (int i = 0; i < N; i++) {
      vector<int>().swap(neighbor[i]);
      vector<int>().swap(edgelist[i]);
    }
    vector< pair<int, int> >().swap(edges);
    vector<string>().swap(labels);
  }
  
  {
    ifstream ifs(data);
    if (ifs.fail()) {
      cerr << "File not found\n";
      exit(0);
    }
    
    ifs >> N;
    D = new int*[N];
    memory_status = true;
    neighbor = new vector<int>[N];
    edgelist = new vector<int>[N];
    
    for (int i = 0; i < N; i++) {
      D[i] = new int[N];
      for (int j = 0; j < N; j++) {
        ifs >> D[i][j];
      }
    }
    
    int diameter = 0;
    
    for (int i = 0; i < N; i++) {
      for (int j = i + 1; j < N; j++) {
        diameter = max(diameter, D[i][j]);
        if (D[i][j] == 1) {
          neighbor[i].push_back(j);
          neighbor[j].push_back(i);
          edges.push_back(make_pair(i, j));
        }
      }
    }
    
    M = edges.size();
    for (int i = 0; i < M; i++) {
      int from = edges[i].first, to = edges[i].second;
      edgelist[from].push_back(i);
      edgelist[to].push_back(i);
    }
    
    calcmdsLayout();
    calc3DLayout();
    calc2DLayout();
    cout << N << " " << M << " " << diameter << " " << dim << endl;
    ifs.close();
  }
  
  //Load Label Data
  loadLabelData();
  //Set Default Node & Edge value
  setNodeEdgeValue();
  
  scale = 1.0f;
  delta = 0.5;
}

void Graph::loadMatrixData_b(const char * data) {
  if(this->isLoaded()) {
    //Free Memory
    {
      if (memory_status) {
        for (int i = 0; i < N; i++) {
          delete[] D[i];
        }
        delete[] D;
        memory_status = false;
      }
      for (int i = 0; i < N; i++) {
        vector<int>().swap(neighbor[i]);
        vector<int>().swap(edgelist[i]);
      }
      vector< pair<int, int> >().swap(edges);
      vector<string>().swap(labels);
    }
  }
  
  
  {
    ifstream ifs(data, ios::out | ios::binary);
    if (ifs.fail()) {
      cerr << "File not found\n";
      exit(0);
    }
    
    int _N;
    ifs.read((char *) &_N, sizeof (int));
    N = _N;
    int * buffer = (int *) malloc((long) sizeof (int)*(N * N));
    ifs.read((char *) buffer, (long) sizeof (int)*N * N);
    ifs.close();
    
    D = new int*[N];
    memory_status = true;
    neighbor = new vector<int>[N];
    edgelist = new vector<int>[N];
    
    for (int i = 0; i < N; i++) {
      D[i] = new int[N];
      for (int j = 0; j < N; j++) {
        D[i][j] = buffer[i + j * N];
      }
    }
    free(buffer);
    
    int diameter = 0;
    for (int i = 0; i < N; i++) {
      for (int j = i + 1; j < N; j++) {
        diameter = max(diameter, D[i][j]);
        if (D[i][j] == 1) {
          neighbor[i].push_back(j);
          neighbor[j].push_back(i);
          edges.push_back(make_pair(i, j));
        }
      }
    }
    M = edges.size();
    
    for (int i = 0; i < M; i++) {
      int from = edges[i].first, to = edges[i].second;
      edgelist[from].push_back(i);
      edgelist[to].push_back(i);
    }
    
    calcmdsLayout();
    calc3DLayout();
    calc2DLayout();
    
    cout << N << " " << M << " " << diameter << " " << dim << endl;
    ifs.close();
  }
  
  //Load Label Data
  loadLabelData();
  //Set Default Node & Edge value
  setNodeEdgeValue();
  scale = 1.0f;
  delta = 0.5;
}

void Graph::loadLayoutData_t(const char * data) {
  //Free Memory
  {
    if (memory_status) {
      for (int i = 0; i < N; i++) {
        delete[] D[i];
      }
      delete[] D;
      memory_status = false;
    }
    for (int i = 0; i < N; i++) {
      vector<int>().swap(neighbor[i]);
      vector<int>().swap(edgelist[i]);
    }
    vector< pair<int, int> >().swap(edges);
    vector<string>().swap(labels);
    delete[] lambdas;
    delete[] P_norms;
    delete[] P;
  }
  
  {
    ifstream ifs(data);
    string str;
    
    if (ifs.fail()) {
      cerr << "File not found\n";
      exit(0);
    }
    
    ifs >> str >> N >> M;
    
    neighbor = new vector<int>[N];
    edgelist = new vector<int>[N];
    
    //load Edges and binary
    for (int i = 0; i < M; i++) {
      int _from, _to;
      ifs >> _from >> _to;
      neighbor[_from].push_back(_to);
      neighbor[_to].push_back(_from);
      edges.push_back(make_pair(_from, _to));
      edgelist[_from].push_back(i);
      edgelist[_to].push_back(i);
    }
    
    ifs >> dim;
    
    lambdas = new float[dim];
    for (int i = 0; i < dim; i++) {
      ifs >> lambdas[i];
    }
    
    P = new float[N * dim];
    P_norms = new float[N];
    
    float pij = 0;
    for (int i = 0; i < N; i++) {
      P_norms[i] = 0;
      for (int j = 0; j < dim; j++) {
        ifs >> pij;
        P[i + j * N] = pij;
        P_norms[i] += pij*pij;
      }
    }
    
    calc3DLayout();
    calc2DLayout();
    cout << N << " " << M << " " << dim << endl;
    ifs.close();
  }
  
  //Load Label Data
  loadLabelData();
  //Set Default Node & Edge value
  setNodeEdgeValue();
  
  scale = 1.0f;
  delta = 0.5;
}

void Graph::loadLayoutData_b(const char * data) {
  const std::string& graphName = this->getName();
  //Free Memory
  {
    if (memory_status) {
      for (int i = 0; i < N; i++) {
        delete[] D[i];
      }
      delete[] D;
      memory_status = false;
    }
    for (int i = 0; i < N; i++) {
      vector<int>().swap(neighbor[i]);
      vector<int>().swap(edgelist[i]);
    }
    vector< pair<int, int> >().swap(edges);
    vector<string>().swap(labels);
    delete[] lambdas;
    delete[] P_norms;
    delete[] P;
  }
  
  {
    ifstream layoutData(data, ios::out | ios::binary);
    
    if (layoutData.fail()) {
      cerr << "File not found\n";
      exit(0);
    }
    
    int _N = 0, _dim = 0;
    layoutData.read((char *) &_N, sizeof (int));
    layoutData.read((char *) &_dim, sizeof (int));
    N = _N;
    dim = _dim;
    
    float * _lambdas = new float[dim];
    layoutData.read((char *) _lambdas, (long) sizeof (float)*(dim));
    lambdas = new float[dim];
    for (int i = 0; i < dim; i++) {
      lambdas[i] = _lambdas[i];
    }
    free(_lambdas);
    
    P = new float[(long) (N)*(long) (dim)];
    layoutData.read((char *) P, (long) sizeof (float)*((long) (N)*(long) (dim)));
    
    P_norms = new float[N];
    float pij = 0;
    for (int i = 0; i < N; i++) {
      P_norms[i] = 0;
      for (int j = 0; j < dim; j++) {
        pij = P[i + j * N];
        P_norms[i] += pij*pij;
      }
    }
    calc3DLayout();
    calc2DLayout();
    layoutData.close();
  }
  
  //Load Edges
  {
    string edgefile = "../data/" + graphName + "/" + graphName + "edges.bin";
    ifstream edgeData(edgefile.c_str(), ios::out | ios::binary);
    if (edgeData.fail()) {
      cerr << "File not found\n";
      exit(0);
    }
    int _M = 0;
    edgeData.read((char *) &_M, sizeof (int));
    cout << _M << endl;
    M = _M;
    D = new int*[N];
    for (int i = 0; i < N; i++) {
      D[i] = new int[N];
    }
    int * _edges = new int[2 * M];
    edgeData.read((char *) _edges, (long) sizeof (int)*(2 * M));
    neighbor = new vector<int>[N];
    edgelist = new vector<int>[N];
    for (int i = 0; i < M; i++) {
      int _from = _edges[2 * i];
      int _to = _edges[2 * i + 1];
      D[_from][_to] = 1;
      D[_to][_from] = 1;
      neighbor[_from].push_back(_to);
      neighbor[_to].push_back(_from);
      edges.push_back(make_pair(_from, _to));
      edgelist[_from].push_back(i);
      edgelist[_to].push_back(i);
    }
    free(_edges);
    edgeData.close();
  }
  //Load Label Data
  loadLabelData();
  //Set Default Node & Edge value
  setNodeEdgeValue();
  cout << N << " " << M << " " << dim << endl;
  
  scale = 1.0f;
  delta = 0.5;
}

int Graph::getNew3DLayout(int id,
                          float pre_x, float pre_y, float pre_z,
                          float new_x, float new_y, float new_z) {
  
  float _pre[3];
  float _new[3];
  _pre[0] = pre_x;
  _pre[1] = pre_y;
  _pre[2] = pre_z;
  _new[0] = new_x;
  _new[1] = new_y;
  _new[2] = new_z;
  
  fvector p(dim);
  float p_norm = P_norms[id] * scale, new_norm = 0, pre_norm = 0;
  
  for (int i = 0; i < dim; i++) {
    p(i) = scale * P[id + i * N];
  }
  
  for (int i = 0; i < 3; i++) {
    new_norm += _new[i] * _new[i];
    pre_norm += _pre[i] * _pre[i];
  }
  
  if (new_norm < p_norm * 0.95f && pre_norm < p_norm * 0.95f) {
    
    float t = 0.001;
    if (new_norm > (1 - t) * p_norm) {
      for (int i = 0; i < 3; i++) {
        _new[i] *= (1 - t) * p_norm / new_norm;
      }
    }
    
    fvector e0(dim), f0(dim), e1(dim), e2(dim), e3(dim);
    
    for (int i = 0; i < dim; i++) {
      e1(i) = E_3D[i + 0 * dim];
      e2(i) = E_3D[i + 1 * dim];
      e3(i) = E_3D[i + 2 * dim];
    }
    
    f0 = p - _pre[0] * e1 - _pre[1] * e2 - _pre[2] * e3;
    float norm_f0 = norm_2(f0);
    e0 = f0 / norm_f0;
    
    float * res = solver3D(_pre, _new, norm_f0, init3D);
    
    if (res == NULL) {
      init3D[0] = 0.9;
      init3D[1] = -0.1;
      init3D[2] = 0.1;
      init3D[3] = -0.1;
      init3D[4] = 0.9;
      init3D[5] = 0.1;
      init3D[6] = 0.1;
      init3D[7] = 0.1;
      init3D[8] = 0.9;
      init3D[9] = 0.4;
      init3D[10] = -0.4;
      init3D[11] = 0.4;
      init3D[12] = -0.4;
      init3D[13] = 0.4;
      init3D[14] = 0.4;
      cout << "NULL" << endl;
      return 0;
    }
    
    fvector e1_new(dim), e2_new(dim), e3_new(dim), r1(dim), r2(dim);
    
    float a10 = (new_x - _pre[0] * res[0] - _pre[1] * res[1] - _pre[2] * res[2]) / norm_f0;
    float a20 = (new_y - _pre[0] * res[3] - _pre[1] * res[4] - _pre[2] * res[5]) / norm_f0;
    float a30 = (new_z - _pre[0] * res[6] - _pre[1] * res[7] - _pre[2] * res[8]) / norm_f0;
    
    e1_new = a10 * e0 + res[0] * e1 + res[1] * e2 + res[2] * e3;
    e2_new = a20 * e0 + res[3] * e1 + res[4] * e2 + res[5] * e3;
    e3_new = a30 * e0 + res[6] * e1 + res[7] * e2 + res[8] * e3;
    
    r1 = res[9] * e1 + res[10] * e2 + res[11] * e3;
    r2 = res[12] * e1 + res[13] * e2 + res[14] * e3;
    
    float error = inner_prod(e1, e1_new) + inner_prod(e2, e2_new) + inner_prod(e3, e3_new);
    if (error < 2.8) return 0;
    
    /*Gram-Schmidt orthonormalization*/
    e1_new = e1_new / norm_2(e1_new);
    e2_new = e2_new - inner_prod(e1_new, e2_new) * e1_new;
    e2_new = e2_new / norm_2(e2_new);
    e3_new = e3_new - inner_prod(e1_new, e3_new) * e1_new - inner_prod(e2_new, e3_new) * e2_new;
    ;
    e3_new = e3_new / norm_2(e3_new);
    
    for (int i = 0; i < dim; i++) {
      E_3D[i + 0 * dim] = e1_new(i);
      E_3D[i + 1 * dim] = e2_new(i);
      E_3D[i + 2 * dim] = e3_new(i);
    }
    
    delete[] Layout3D;
    Layout3D = new float[N * 3];
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, 3, dim, scale, P, N, E_3D, dim, 0.0, Layout3D, N);
    return 1;
  } else {
    return 0;
  }
}

int Graph::getNew2DLayout(int id,
                          float pre_x, float pre_y,
                          float new_x, float new_y) {
  
  float _pre[2];
  float _new[2];
  _pre[0] = pre_x;
  _pre[1] = pre_y;
  _new[0] = new_x;
  _new[1] = new_y;
  
  fvector p(dim);
  float p_norm = P_norms[id] * scale, new_norm = 0, pre_norm = 0;
  
  for (int i = 0; i < dim; i++) {
    p(i) = scale * P[id + i * N];
  }
  
  for (int i = 0; i < 2; i++) {
    new_norm += _new[i] * _new[i];
    pre_norm += _pre[i] * _pre[i];
  }
  
  if (new_norm < p_norm * 0.95f && pre_norm < p_norm * 0.95f) {
    
    float t = 0.001;
    
    if (new_norm > (1 - t) * p_norm) {
      for (int i = 0; i < 3; i++) {
        _new[i] *= (1 - t) * p_norm / new_norm;
      }
    }
    
    fvector e0(dim), f0(dim), e1(dim), e2(dim);
    
    for (int i = 0; i < dim; i++) {
      e1(i) = E_2D[i + 0 * dim];
      e2(i) = E_2D[i + 1 * dim];
    }
    
    f0 = p - _pre[0] * e1 - _pre[1] * e2;
    float norm_f0 = norm_2(f0);
    e0 = f0 / norm_f0;
    
    float * res = solver2D(_pre, _new, norm_f0, init2D);
    
    if (res == NULL) {
      init2D[0] = 0.9;
      init2D[1] = -0.1;
      init2D[2] = 0.1;
      init2D[3] = 0.9;
      init2D[4] = 0.4;
      init2D[5] = 0.4;
      cout << "NULL" << endl;
      return 0;
    }
    
    fvector e1_new(dim), e2_new(dim), r(dim);
    
    float a10 = (new_x - _pre[0] * res[0] - _pre[1] * res[1]) / norm_f0;
    float a20 = (new_y - _pre[0] * res[2] - _pre[1] * res[3]) / norm_f0;
    
    e1_new = a10 * e0 + res[0] * e1 + res[1] * e2;
    e2_new = a20 * e0 + res[2] * e1 + res[3] * e2;
    
    r = res[4] * e1 + res[5] * e2;
    
    
    float error = inner_prod(e1, e1_new) + inner_prod(e2, e2_new);
    
    if (error < 1.6) return 0;
    
    //Gram-Schmidt orthonormalization
    e1_new = e1_new / norm_2(e1_new);
    e2_new = e2_new - inner_prod(e1_new, e2_new) * e1_new;
    e2_new = e2_new / norm_2(e2_new);
    
    for (int i = 0; i < dim; i++) {
      E_2D[i + 0 * dim] = e1_new(i);
      E_2D[i + 1 * dim] = e2_new(i);
    }
    
    delete[] Layout2D;
    Layout2D = new float[N * 2];
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, 2, dim,
                scale, P, N, E_2D, dim, 0.0, Layout2D, N);
    
    return 1;
  } else {
    return 0;
  }
}