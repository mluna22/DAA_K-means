/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file k-means.h
 * @brief KMeans class
 *        This class implements the k-means algorithm
 */

#ifndef K_MEANS_H
#define K_MEANS_H

#include <vector>
#include <set>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "utilities.h"

class KMeans {
 public:
  KMeans();
  std::vector<Solution> solve(const Problem& data, int k);
};

KMeans::KMeans() {}

std::vector<Solution> KMeans::solve(const Problem& points, int k) {
  // Seleccionar K puntos aleatorios como centroides iniciales
  std::set<int> random_centroids;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, points.size() - 1);
  while (random_centroids.size() < k) {
    random_centroids.insert(dis(gen));
  }

  // Los primeros clusters corresponden con los centroides iniciales
  std::vector<Point> centroids;
  for (int index: random_centroids) {
    centroids.push_back(points[index]);
  }
  std::vector<Solution> solutions;
  Solution solution(k, points.dimensions());
  Solution new_solution(k, points.dimensions());

  // Recorremos todos los puntos y centroides para asignar cada punto al centroide más cercano
  for (int i{0}; i < points.size(); ++i) {  // Recorrer todos los puntos
    double min_distance{euclidean_distance(points[i], centroids[0])};
    int closest_centroid_index{0};
    for (int j{0}; j < k; ++j) {  // Recorrer todos los centroides
      double distance{euclidean_distance(points[i], centroids[j])};
      if (distance < min_distance) {
        min_distance = distance;
        closest_centroid_index = j;
      }
    }
    new_solution[closest_centroid_index].push_back(points[i]);  // Asignar el punto al cluster más cercano
  }

  // Calcular los nuevos centroides
  for (int i{0}; i < k; ++i) {  // Recorrer todos los centroides
    centroids[i] = new_solution.centroid(i);
  }

  // Si los clusters no cambian, terminar
  while (solution != new_solution) {
    solutions.push_back(new_solution);
    solution = new_solution;
    new_solution = Solution(k, points.dimensions());
    // Recorremos todos los puntos y centroides para asignar cada punto al centroide más cercano
    for (int i{0}; i < points.size(); ++i) {  // Recorrer todos los puntos
      double min_distance{euclidean_distance(points[i], centroids[0])};
      int closest_centroid_index{0};
      for (int j{0}; j < k; ++j) {  // Recorrer todos los centroides
        double distance{euclidean_distance(points[i], centroids[j])};
        if (distance < min_distance) {
          min_distance = distance;
          closest_centroid_index = j;
        }
      }
      new_solution[closest_centroid_index].push_back(points[i]);  // Asignar el punto al cluster más cercano
    }

    // Calcular los nuevos centroides
    for (int i{0}; i < k; ++i) {  // Recorrer todos los centroides
      centroids[i] = new_solution.centroid(i);
    }
  }

  return solutions;
}

#endif  // K_MEANS_H