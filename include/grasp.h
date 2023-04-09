/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file grasp.h
 * @brief Grasp class
 *        This class implements the grasp algorithm
 */

#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include <set>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "utilities.h"

class Grasp {
 public:
  Grasp();
  // std::vector<Point> createLRC(const Problem& points, const Solution& solution, int lrc_size);
  std::vector<Solution> solve(const Problem& points, int k, int lrc_size);
 private:
  void heuristic(int index, std::vector<int>& lrc, int lrc_size, Solution& clusters);
};

Grasp::Grasp() {}

void Grasp::heuristic(int index, std::vector<int>& lrc, int lrc_size, Solution& clusters) {
  
}

// std::vector<Point> Grasp::createLRC(const Problem& points, const Solution& solution, int lrc_size) {
//   // Creamos el LRC
//   std::vector<Point> lrc;

//   // Calculamos la distancia para cada punto al punto de la solución más cercano
//   std::vector<double> distances(points.size());
//   for (int i{0}; i < points.size(); ++i) {  // Para cada punto
//     double min_distance{euclidean_distance(points[i], solution[0][0])};
//     for (int j{0}; j < solution.size(); ++j) {  // Para cada cluster
//       for (int k{0}; k < solution[j].size(); ++k) {  
//         double distance{euclidean_distance(points[i], solution[j][k])};
//         if (distance < min_distance && distance != 0) {
//           min_distance = distance;
//         }
//       }
//     }
//     distances[i] = min_distance;
//     std::cout << "Distancia " << i << ": " << distances[i] << std::endl;
//   }
// }

std::vector<Solution> Grasp::solve(const Problem& points, int k, int lrc_size) {
  bool condition{true};
  while(condition) {
    // Creamos la solución inicial
    Solution solution(points.dimensions());
    condition = false;
  }
  return std::vector<Solution>();
}


#endif  // GRASP_H