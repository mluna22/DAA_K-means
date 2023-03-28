/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file utilities.h
 * @brief Type definitions
 *       This file contains the Problem and Solution type definitions
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <cmath>

typedef std::vector<double> Point;
typedef std::vector<Point> Cluster;

double euclidean_distance(const Point& a, const Point& b) {
  double distance{0};
  for (int i{0}; i < a.size(); ++i) {
    distance += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return sqrt(distance);
}

class Problem {
 public:
  Problem(int n, int d) {
    for (int i{0}; i < n; ++i) {
      points_.push_back(Point(d));
    }
  }

  const Point& operator[](int i) const {
    return points_[i];
  }
  Point& operator[](int i) {
    return points_[i];
  }

  const int size() const {
    return points_.size();
  }

  const int dimensions() const {
    return points_[0].size();
  }
 private:
  std::vector<Point> points_;
};

class Solution {
 public:
  Solution(int k, int d) {
    dimensions_ = d;
    for (int i{0}; i < k; ++i) {
      clusters_.push_back(Cluster());
    }
  }
  std::vector<Point>& operator[](int i) {
    return clusters_[i];
  }

  const int size() {
    return clusters_.size();
  }

  Point centroid(int i) {
    Point centroid;
    for (int j{0}; j < dimensions_; ++j) {  // for each dimension
      double sum{0};
      for (int k{0}; k < clusters_[i].size(); ++k) {  // for each point in the cluster
        sum += clusters_[i][k][j];
      }
      centroid.push_back(sum / clusters_[i].size());
    }
    return centroid;
  }

  double SSE() {
    double sse{0};
    for (int i{0}; i < clusters_.size(); ++i) {  // for each cluster
      Point centroid = this->centroid(i);
      for (int j{0}; j < clusters_[i].size(); ++j) {
        double distance = euclidean_distance(clusters_[i][j], centroid);
        sse += distance * distance;
      }
    }
    return sse;
  }

  bool operator==(const Solution& other) {
    if (clusters_.size() != other.clusters_.size()) return false;
    for (int i{0}; i < clusters_.size(); ++i) {
      if (clusters_[i].size() != other.clusters_[i].size()) return false;
      for (int j{0}; j < clusters_[i].size(); ++j) {
        if (clusters_[i][j].size() != other.clusters_[i][j].size()) return false;
        for (int k{0}; k < clusters_[i][j].size(); ++k) {
          if (clusters_[i][j][k] != other.clusters_[i][j][k]) {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool operator!=(const Solution& other) {
    return !(*this == other);
  }
 private:
  std::vector<Cluster> clusters_;
  int dimensions_;
};

#endif  // UTILITIES_H