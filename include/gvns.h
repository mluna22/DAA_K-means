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
 *        This class implements the gvns algorithm
 */

#ifndef GVNS_H
#define GVNS_H

#include <vector>
#include <set>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "solution.h"

class GVNS {
 public:
  GVNS();
  std::vector<Solution> solve(const Problem& points, int k, bool rvnd = false);
};

GVNS::GVNS() {}

std::vector<Solution> GVNS::solve(const Problem& points, int k, bool rvnd) {
  //Preprocesamiento
  std::vector<Solution> solutions;

  // Construimos una solución aleatoria con la fase constructiva de GRASP
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
        if (distance < distances[i]) {
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
    for (int i{0}; i < 3; ++i) {
      lrc.push_back(sorted_points[i]);
    }
    // Seleccionamos un punto aleatorio del LRC
    std::uniform_int_distribution<> dis2(0, lrc.size() - 1);
    solution.push_back(points[lrc[dis2(gen)]]);
  }

  int iterations_without_improvement{0};
  int iterations{0};
  int shake_size{1};
  while(iterations_without_improvement < 200 && iterations < 3000) {
    shake_size = 1;
    while (shake_size <= solution.size()) {
      Solution new_solution = solution;
      // Shaking
      // Seleccionamos aleatoriamente shake_size puntos de la solución
      std::vector<int> selected_points;
      std::uniform_int_distribution<> dis3(0, new_solution.size() - 1);
      while (selected_points.size() < shake_size) {
        int point{dis3(gen)};
        if (std::find(selected_points.begin(), selected_points.end(), point) == selected_points.end()) {
          selected_points.push_back(point);
        }
      }
      // Seleccionamos aleatoriamente shake_size puntos de los puntos que no están en la solución
      std::vector<int> new_problem_points;
      std::uniform_int_distribution<> dis4(0, points.size() - 1);
      while (new_problem_points.size() < shake_size) {
        int point{dis4(gen)};
        if ((std::find(selected_points.begin(), selected_points.end(), point) == selected_points.end()) &&
          !new_solution.isInSolution(points[point])) {
          new_problem_points.push_back(point);
        }
      }
      // Intercambiamos los puntos seleccionados
      for (int i{0}; i < shake_size; ++i) {
        new_solution[selected_points[i]] = points[new_problem_points[i]];
      }

      if (rvnd) {
        // new_solution = new_solution.rvnd(points);
      } else {
        new_solution = new_solution.local_search(points);
      }
      // Movimiento
      if (new_solution.evaluate(points) < solution.evaluate(points)) {
        solution = new_solution;
        shake_size = 1;
      } else {
        shake_size++;
      }
    }
    // Actualización de la solución
    iterations_without_improvement++;
    iterations++;
    if (solutions.empty() || solution.evaluate(points) < solutions[solutions.size() - 1].evaluate(points)) {
      solutions.push_back(solution);
      iterations_without_improvement = 0;
    }
  }
  return solutions;
  
  // RVNS
}

#endif  // GVNS_H