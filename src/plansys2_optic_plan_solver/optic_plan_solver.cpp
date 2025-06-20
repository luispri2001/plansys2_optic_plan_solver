// Copyright 2021 PySC21
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <sys/stat.h>
#include <sys/types.h>

#include <filesystem>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "plansys2_optic_plan_solver/optic_plan_solver.hpp"

namespace plansys2
{

OPTICPlanSolver::OPTICPlanSolver()
{
}

std::optional<plansys2_msgs::msg::Plan>
OPTICPlanSolver::getPlan(
  const std::string & domain, const std::string & problem,
  const std::string & node_namespace)
{
  if (node_namespace != "") {
    std::filesystem::path tp = std::filesystem::temp_directory_path();
    for (auto p : std::filesystem::path(node_namespace) ) {
      if (p != std::filesystem::current_path().root_directory()) {
        tp /= p;
      }
    }
    std::filesystem::create_directories(tp);
  }

  plansys2_msgs::msg::Plan ret;
  std::ofstream domain_out("/tmp/" + node_namespace + "/domain.pddl");
  domain_out << domain;
  domain_out.close();

  std::ofstream problem_out("/tmp/" + node_namespace + "/problem.pddl");
  problem_out << problem;
  problem_out.close();

  system(
    ("optic -N /tmp/" + node_namespace + "/domain.pddl /tmp/" +
    node_namespace + "/problem.pddl > /tmp/" + node_namespace + "/plan_optic").c_str());
  std::string line_optic;
  std::ifstream plan_fileoptic("/tmp/" + node_namespace + "/plan_optic");
  bool solution_optic = false;

  if (plan_fileoptic.is_open()) {
    while (getline(plan_fileoptic, line_optic)) {
      if (!solution_optic) {
        if (line_optic.find(";;;; Solution Found") != std::string::npos) {
          solution_optic = true;
        }
      } else if (isdigit(line_optic.front())) {
        plansys2_msgs::msg::PlanItem item;
        std::cout << " optic:" << line_optic << std::endl;
        size_t colon_pos = line_optic.find(":");
        size_t colon_par = line_optic.find(")");
        size_t colon_bra = line_optic.find("[");

        std::string time = line_optic.substr(0, colon_pos);
        std::string action = line_optic.substr(colon_pos + 2, colon_par - colon_pos - 1);
        std::string duration = line_optic.substr(colon_bra + 1);
        duration.pop_back();

        item.time = std::stof(time);
        item.action = action;
        item.duration = std::stof(duration);

        ret.items.push_back(item);
      }
    }
    plan_fileoptic.close();
  }


  if (ret.items.empty()) {
    return {};
  } else {
    return ret;
  }
}

std::string
OPTICPlanSolver::check_domain(
  const std::string & domain,
  const std::string & node_namespace)
{
  if (node_namespace != "") {
    mkdir(("/tmp/" + node_namespace).c_str(), ACCESSPERMS);
  }

  std::ofstream domain_out("/tmp/" + node_namespace + "/check_domain.pddl");
  domain_out << domain;
  domain_out.close();

  std::ofstream problem_out("/tmp/" + node_namespace + "/check_problem.pddl");
  problem_out << "(define (problem void) (:domain plansys2))";
  problem_out.close();

  system(
    ("optic -N /tmp/" + node_namespace + "/check_domain.pddl /tmp/" +
    node_namespace + "/check_problem.pddl > /tmp/" + node_namespace + "/check.out").c_str());

  std::ifstream plan_file("/tmp/" + node_namespace + "/check.out");

  std::string result((std::istreambuf_iterator<char>(plan_file)),
    std::istreambuf_iterator<char>());

  return result;
}

}  // namespace plansys2

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(plansys2::OPTICPlanSolver, plansys2::PlanSolverBase);
