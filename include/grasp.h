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
#include "solution.h"

class Grasp {
 public:
  Grasp();
  std::vector<Solution> solve(const Problem& points, int k, int lrc_size);
};

Grasp::Grasp() {}

std::vector<Solution> Grasp::solve(const Problem& points, int k, int lrc_size) {
  //Preprocesamiento
  std::vector<Solution> solutions;
  int condition{0};
  while(condition < 200) {
    // Fase constructiva
    // Seleccionar un punto aleatorio como solución inicial
    Solution solution(points.dimensions());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, points.size() - 1);
    solution.push_back(points[dis(gen)]);
    // Mientras no se haya alcanzado el número de puntos de servicio
    while (solution.size() < k) {
      std::vector<int> lrc;
      std::vector<double> distances(points.size());
      // Buscamos la distancia mínima de cada punto a la solución
      // Añadimos al LRC los puntos con la distancia mínima más alta
      for (int i{0}; i < points.size(); ++i) { // Para cada punto
        for (int j{0}; j < solution.size(); ++j) { // Para cada punto de la solución
          double distance{euclidean_distance(points[i], solution[j])};
          if (distance < distances[i] && distance != 0) {
            distances[i] = distance;
          }
        }
      }
      // Ordenamos los puntos por distancia
      std::vector<int> sorted_points(points.size());
      std::iota(sorted_points.begin(), sorted_points.end(), 0);
      std::sort(sorted_points.begin(), sorted_points.end(), [&distances](int a, int b) {
        return distances[a] < distances[b];
      });
      // Añadimos los puntos al LRC
      for (int i{0}; i < lrc_size; ++i) {
        lrc.push_back(sorted_points[i]);
      }
      // Seleccionamos un punto aleatorio del LRC
      std::uniform_int_distribution<> dis2(0, lrc.size() - 1);
      solution.push_back(points[lrc[dis2(gen)]]);
    }

    // Postprocesamiento

    // Actualización de la solución
    condition++;
    if (solutions.empty() || solution.evaluate(points) < solutions[solutions.size() - 1].evaluate(points)) {
      solutions.push_back(solution);
      condition = 0;
    }
  }
  return solutions;
}


#endif  // GRASP_H