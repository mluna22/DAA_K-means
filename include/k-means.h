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
#include "solution.h"

class KMeans {
 public:
  KMeans();
  std::vector<Solution> solve(const Problem& points, int k);
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
  Solution solution(points.dimensions());
  for (auto it: random_centroids) {
    solution.push_back(points[it]);
  }
  std::vector<Cluster> clusters(k);
  std::vector<Solution> solutions;
  Solution new_solution(points.dimensions());

  // Recorremos todos los puntos y centroides para asignar cada punto al centroide más cercano
  for (int i{0}; i < points.size(); ++i) {  // Recorrer todos los puntos
    double min_distance{INFINITY};
    int closest_centroid_index{0};
    for (int j{0}; j < k; ++j) {  // Recorrer todos los centroides
      double distance{euclidean_distance(points[i], solution[j])};
      if (distance < min_distance) {
        min_distance = distance;
        closest_centroid_index = j;
      }
    }
    clusters[closest_centroid_index].push_back(points[i]);  // Asignar el punto al cluster más cercano
  }

  // Calcular los nuevos centroides
  for (int i{0}; i < k; ++i) {  // Recorrer todos los centroides
    Point centroid;
    for (int j{0}; j < points.dimensions(); ++j) {  // for each dimension
      double sum{0};
      for (int k{0}; k < clusters[i].size(); ++k) {  // for each point in the cluster
        sum += clusters[i][k][j];
      }
      centroid.push_back(sum / clusters[i].size());
    }
    new_solution.push_back(centroid);
  }

  // Repetir. Si los clusters no cambian, terminar
  while (solution != new_solution) {
    solutions.push_back(new_solution);
    solution = new_solution;
    new_solution = Solution(points.dimensions());
    for (int i{0}; i < k; ++i) {
      clusters[i].clear();
    }
    // Recorremos todos los puntos y centroides para asignar cada punto al centroide más cercano
    for (int i{0}; i < points.size(); ++i) {  // Recorrer todos los puntos
      double min_distance{euclidean_distance(points[i], solution[0])};
      int closest_centroid_index{0};
      for (int j{0}; j < k; ++j) {  // Recorrer todos los centroides
        double distance{euclidean_distance(points[i], solution[j])};
        if (distance < min_distance) {
          min_distance = distance;
          closest_centroid_index = j;
        }
      }
      clusters[closest_centroid_index].push_back(points[i]);  // Asignar el punto al cluster más cercano
    }

    // Calcular los nuevos centroides
    for (int i{0}; i < k; ++i) {  // Recorrer todos los centroides
      Point centroid;
      for (int j{0}; j < points.dimensions(); ++j) {  // for each dimension
        double sum{0};
        for (int k{0}; k < clusters[i].size(); ++k) {  // for each point in the cluster
          sum += clusters[i][k][j];
        }
        centroid.push_back(sum / clusters[i].size());
      }
      new_solution.push_back(centroid);
    }
  }

  return solutions;
}

#endif  // K_MEANS_H