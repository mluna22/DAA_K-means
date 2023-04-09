/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file utilities.h
 * @brief Utilities
 *        This file contains the utilities used in the program
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <cmath>

typedef std::vector<double> Point;
typedef std::vector<Point> Cluster;

/**
 * @brief Calculates the euclidean distance between two points
 * @param a First point
 * @param b Second point
 * @return Euclidean distance between a and b
 */
double euclidean_distance(const Point& a, const Point& b) {
  double distance{0};
  for (int i{0}; i < a.size(); ++i) {
    distance += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return sqrt(distance);
}

#endif  // UTILITIES_H