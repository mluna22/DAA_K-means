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
#include "grasp.h"
#include "gvns.h"

#define N_INSTANCES 5

std::ostream& printKMeans(std::ostream& os, std::string instance, Problem& problem, KMeans algorithm, bool debug = false) {
  for (int i{0}; i < N_INSTANCES; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Solution> solutions = algorithm.solve(problem, problem.size()/10 < 2 ? 2 : problem.size()/10);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    os << instance << "," << problem.size() << "," << solutions[solutions.size() - 1].size() << "," << i + 1 << "," << solutions[solutions.size() - 1].evaluate(problem) << "," << elapsed_seconds.count() << std::endl;
    if (debug) {
      for (int j{0}; j < solutions[solutions.size() - 1].size(); ++j) {
        for (int k{0}; k < solutions[solutions.size() - 1][j].size(); ++k) {
          os << solutions[solutions.size() - 1][j][k] << " ";
        } os << std::endl;
      }
    }
  }
  return os;
}

std::ostream& printGrasp(std::ostream& os, std::string instance, Problem& problem, Grasp algorithm, int lrc_size, bool debug = false) {
  for (int i{0}; i < N_INSTANCES; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Solution> solutions = algorithm.solve(problem, problem.size()/10 < 2 ? 2 : problem.size()/10, lrc_size);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    os << instance << "," << problem.size() << "," << solutions[solutions.size() - 1].size() << "," << lrc_size << "," << i + 1 << "," << solutions[solutions.size() - 1].evaluate(problem) << "," << elapsed_seconds.count() << std::endl;
    if (debug) {
      for (int j{0}; j < solutions[solutions.size() - 1].size(); ++j) {
        for (int k{0}; k < solutions[solutions.size() - 1][j].size(); ++k) {
          os << solutions[solutions.size() - 1][j][k] << " ";
        } os << std::endl;
      }
    }
  }
  return os;
}

std::ostream& printGVNS(std::ostream& os, std::string instance, Problem& problem, GVNS algorithm, bool rvnd = false, bool debug = false) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Solution> solutions = algorithm.solve(problem, problem.size()/10 < 2 ? 2 : problem.size()/10, rvnd);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  for (int i{0}; i < N_INSTANCES; ++i) {
    os << instance << "," << problem.size() << "," << solutions[solutions.size() - 1].size() << "," << solutions[solutions.size() - 1].size() << "," << i + 1 << "," << solutions[solutions.size() - 1].evaluate(problem) << "," << elapsed_seconds.count() << std::endl;
    if (debug) {
      for (int j{0}; j < solutions[solutions.size() - 1].size(); ++j) {
        for (int k{0}; k < solutions[solutions.size() - 1][j].size(); ++k) {
          os << solutions[solutions.size() - 1][j][k] << " ";
        } os << std::endl;
      }
    }
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
  bool debug = (argc == 3 && std::string(argv[2]) == "1");
  std::string instance_folder = argv[1];
  KMeans kmeans;
  std::cout << "Algoritmo K-Means" << std::endl;
  std::cout << "Problema,m,k,Ejecución,SSE,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printKMeans(std::cout, instance_path, matrix, kmeans, debug);
  }

  Grasp grasp;
  std::cout << "Algoritmo GRASP" << std::endl;
  std::cout << "Problema,m,k,|LRC|,Ejecución,SSE,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printGrasp(std::cout, instance_path, matrix, grasp, 3, debug);
  }

  GVNS gvns;
  std::cout << "Algoritmo GVNS" << std::endl;
  std::cout << "Problema,m,k,kmax,Ejecución,SSE,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printGVNS(std::cout, instance_path, matrix, gvns, 3, debug);
  }

  return 0;
}