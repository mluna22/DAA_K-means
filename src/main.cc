/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file main.cc
 * @brief Clustering Problem Client
 *        This program solves the clustering problem using k-means and grasp
*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <chrono>

#include "k-means.h"

template <class T>
std::ostream& printResults(std::ostream& os, std::string instance, Problem& problem, T algorithm) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Solution> solutions = algorithm.solve(problem, problem.size()/10 < 2 ? 2 : problem.size()/10);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  for (int i{0}; i < solutions.size(); ++i) {
    os << instance << "," << problem.size() << "," << solutions[i].size() << "," << i + 1 << "," << solutions[i].SSE() << "," << elapsed_seconds.count() << std::endl;
  }
  return os;
}

Problem loadProblem(std::string instance_path) {
  std::ifstream file(instance_path);
  if (!file.is_open()) {
    std::cout << "Error opening file " << instance_path << std::endl;
    throw;
  }
  int m, n;
  file >> m >> n;
  Problem problem(m, n);
  for (int i{0}; i < m; ++i) {
    for (int j{0}; j < n; ++j) {
      file >> problem[i][j];
    }
  }
  file.close();
  return problem;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <instance_folder> [<output_file>]" << std::endl;
    return 1;
  }
  std::ofstream output;
  // if (argc == 3) {
  //   output.open(argv[2]);
  // } else {
  //   output.open("out.csv");
  // }
  std::string instance_folder = argv[1];
  KMeans kmeans;
  std::cout << "Algoritmo K-Means" << std::endl;
  std::cout << "Problema,m,k,Ejecución,SSE,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printResults(std::cout, instance_path, matrix, kmeans);
  }

  return 0;
}