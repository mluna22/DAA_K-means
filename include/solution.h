/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file problem.h
 * @brief Solution Class
 *        This file contains the Solution class declaration
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <cmath>
#include "problem.h"

/**
 * @brief Defines a solution to the clustering problem
*/
class Solution {
 public:
  /**
   * @brief Creates a new solution
   * @param d Number of dimensions
  */
  Solution(int d) {
    dimensions_ = d;
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

  /**
   * @brief Calculates the sum of distances of the solution
   */
  const double evaluate(const Problem& problem) {
    double sum_of_distances{0};
    for (int i{0}; i < problem.size(); ++i) {  // por cada punto
      double min_distance{INFINITY};
      for (int j{0}; j < points_.size(); ++j) {  // por cada punto de servicio
        double distance{euclidean_distance(problem[i], points_[j])};
        if (distance < min_distance) {
          min_distance = distance;
        }
      }
      sum_of_distances += min_distance;
    }
    return sum_of_distances;
  }

  /**
   * @brief Calculates the sum of distances of the solution and the distances of each point to the solution
   */
  const double evaluate(const Problem& problem, DistanceIndex& distances) {
    double sum_of_distances{0};
    distances = DistanceIndex(problem.size(), {0, 0});
    for (int i{0}; i < problem.size(); ++i) {  // por cada punto
      double min_distance{INFINITY};
      for (int j{0}; j < points_.size(); ++j) {  // por cada punto de servicio
        double distance{euclidean_distance(problem[i], points_[j])};
        if (distance < min_distance) {
          min_distance = distance;
          distances[i] = {distance, j};
        }
      }
      sum_of_distances += min_distance;
    }
    double result_check{0};
    for (int i{0}; i < problem.size(); ++i) {
      result_check += distances[i].first;
    }
    
    return sum_of_distances;
  }

  const bool operator==(const Solution& other) {
    if (points_.size() != other.points_.size()) return false;
    for (int i{0}; i < points_.size(); ++i) { // por cada punto
      if (points_[i].size() != other.points_[i].size()) return false;
      for (int j{0}; j < points_[i].size(); ++j) { // por cada dimension
        if (fabs(points_[i][j] - other.points_[i][j]) > 0.001) return false;
      }
    }
    return true;
  }

  const bool operator!=(const Solution& other) {
    return !(*this == other);
  }

  void push_back(Point point) {
    points_.push_back(point);
  }

  const int dimensions() {
    return dimensions_;
  }

  Solution local_search(const Problem& problem) {
    // Intercambio, inserción y eliminación
    DistanceIndex distances;
    Solution best_solution(*this);
    Solution new_solution(*this);
    double best_solution_value{evaluate(problem, distances)};
    double new_solution_value{best_solution_value};
    DistanceIndex best_solution_distances{distances};
    DistanceIndex new_solution_distances{distances};
    do {
      best_solution = new_solution;
      best_solution_value = new_solution_value;
      best_solution_distances = new_solution_distances;

      new_solution = best_solution.exchange_search(problem, new_solution_distances);
      new_solution_value = new_solution.evaluate(problem, new_solution_distances);
      if (new_solution_value < best_solution_value) {
        continue;
      }
      new_solution = best_solution.insertion_search(problem, new_solution_distances);
      new_solution_value = new_solution.evaluate(problem, new_solution_distances);
      if (new_solution_value < best_solution_value) {
        continue;
      }
      new_solution = best_solution.elimination_search(problem, new_solution_distances);
      new_solution_value = new_solution.evaluate(problem, new_solution_distances);
    } while (new_solution_value < best_solution_value);
    distances = best_solution_distances;
    return best_solution;
  }
  
 private:
  // Centroids if kmeans, points of service if grasp
  std::vector<Point> points_;
  int dimensions_;

  const double evaluate_insertion(const Problem& points, DistanceIndex& distances, int new_index_from_points) {
    double sum_of_distances{0};
    for (int i{0}; i < points.size(); ++i) {
      double distance{euclidean_distance(points[i], points[new_index_from_points])};
      if (distance < distances[i].first) {
        distances[i] = {distance, new_index_from_points};
      }
      sum_of_distances += distances[i].first;
    }
    return sum_of_distances;
  }

  const double evaluate_elimination(const Problem& points, DistanceIndex& distances, int old_index_from_solution) {
    double sum_of_distances{0};
    for (int i{0}; i < points.size(); ++i) {
      if (distances[i].second == old_index_from_solution) {
        double min_distance{INFINITY};
        for (int j{0}; j < this->points_.size(); ++j) {
          if (j != old_index_from_solution) {
            double distance{euclidean_distance(points[i], this->points_[j])};
            if (distance < min_distance) {
              min_distance = distance;
              distances[i] = {distance, j};
            }
          }
        }
      }
      sum_of_distances += distances[i].first;
    }
    return sum_of_distances;
  }

  const double evaluate_exchange(const Problem& points, DistanceIndex& distances, int old_index_from_solution, int new_index_from_points) {
    double sum_of_distances{0};
    for (int i{0}; i < points.size(); ++i) {
      if (distances[i].second == old_index_from_solution) {
        double min_distance{INFINITY};
        for (int j{0}; j < this->points_.size(); ++j) {
          if (j != old_index_from_solution) {
            double distance{euclidean_distance(points[i], this->points_[j])};
            if (distance < min_distance) {
              min_distance = distance;
              distances[i] = {distance, j};
            }
          }
        }
      }
      double new_distance = euclidean_distance(points[i], points[new_index_from_points]);
      if (distances[i].first > new_distance) {
        distances[i] = {new_distance, new_index_from_points};
      }
      sum_of_distances += distances[i].first;
    }
    return sum_of_distances;
  }

  Solution insertion_search(const Problem& problem, DistanceIndex& distances) {
    Solution best_solution(*this);
    double best_solution_value{sum_distances(distances)};
    float penalty{0.2};  // 20% de penalización
    for (int j{0}; j < problem.size(); ++j) { // por cada punto
      Solution new_solution(*this);
      DistanceIndex new_distances = distances;
      double new_solution_value = new_solution.evaluate_insertion(problem, new_distances, j);
      if (new_solution_value < best_solution_value * (1 - penalty)) {
        new_solution.push_back(problem[j]);
        best_solution = new_solution;
        best_solution_value = new_solution_value;
      }
    }
    return best_solution;
  }

  Solution elimination_search(const Problem& problem, DistanceIndex& distances) {
    Solution best_solution(*this);
    double best_solution_value{sum_distances(distances)};
    float boost{0.05};  // 5% de aumento
    for (int i{0}; i < points_.size(); ++i) { // por cada punto de la solución
      Solution new_solution(*this);
      DistanceIndex new_distances = distances;
      double new_solution_value = new_solution.evaluate_elimination(problem, new_distances, i);
      if (new_solution_value * (1 - boost) < best_solution_value) {
        new_solution.points_.erase(new_solution.points_.begin() + i);
        best_solution = new_solution;
        best_solution_value = new_solution_value;
      }
    }
    return best_solution;
  }

  Solution exchange_search(const Problem& problem, DistanceIndex& distances) {
    Solution best_solution(*this);
    double best_solution_value{sum_distances(distances)};
    for (int i{0}; i < points_.size(); ++i) { // por cada punto de la solución
      for (int j{0}; j < problem.size(); ++j) { // por cada punto
        Solution new_solution(*this);
        DistanceIndex new_distances = distances;
        double new_solution_value = new_solution.evaluate_exchange(problem, new_distances, i, j);
        if (new_solution_value < best_solution_value) {
          new_solution[i] = problem[j];
          best_solution = new_solution;
          best_solution_value = new_solution_value;
        }
      }
    }
    return best_solution;
  }
};

#endif  // SOLUTION_H