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
  std::vector<Solution> solve(const Problem& points, int k, int lrc_size);
 private:
  void insert_lrc(int index, std::vector<int>& lrc, int lrc_size, Solution& clusters);
};

Grasp::Grasp() {}

void Grasp::insert_lrc(int index, std::vector<int>& lrc, int lrc_size, Solution& clusters) {
  if (lrc.size() < lrc_size) {
    lrc.push_back(index);
  } else {
    int max_index{0};
    double max_distance{euclidean_distance(clusters.centroid(index), clusters.centroid(lrc[0]))};
    for (int i{1}; i < lrc_size; ++i) {
      double distance{euclidean_distance(clusters.centroid(index), clusters.centroid(lrc[i]))};
      if (distance > max_distance) {
        max_distance = distance;
        max_index = i;
      }
    }
    if (max_distance > euclidean_distance(clusters.centroid(index), clusters.centroid(lrc[max_index]))) {
      lrc[max_index] = index;
    }
  }
}

std::vector<Solution> Grasp::solve(const Problem& points, int k, int lrc_size) {
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

  // Repetir hasta 5 iteraciones
  int i = 0;
  while (i < 5) {
    // La lista restringida de candidatos será los indices de los clusters mas cercanos a los puntos
    std::vector<int> lrc;

    // Recorremos todos los puntos y centroides para asignar cada punto al centroide más cercano
    for (int i{0}; i < points.size(); ++i) {  // Recorrer todos los puntos
      int farthest_centroid_index{0};
      lrc.clear();
      for (int j{0}; j < k; ++j) {  // Metemos en lrc los centroides más cercanos al punto
        insert_lrc(j, lrc, lrc_size, new_solution);
      }
      dis = std::uniform_int_distribution<>(0, lrc.size() - 1);
      farthest_centroid_index = lrc[dis(gen)];
      new_solution[farthest_centroid_index].push_back(points[i]);  // Asignar el punto al cluster más cercano
    }

    // Calcular los nuevos centroides
    for (int i{0}; i < k; ++i) {  // Recorrer todos los centroides
      centroids[i] = new_solution.centroid(i);
    }

    solutions.push_back(new_solution);
    solution = new_solution;
    i++;
  }

  return solutions;
}

#endif  // GRASP_H