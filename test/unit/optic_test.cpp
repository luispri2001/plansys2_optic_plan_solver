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

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "gtest/gtest.h"
#include "plansys2_optic_plan_solver/optic_plan_solver.hpp"

#include "pluginlib/class_loader.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "plansys2_core/PlanSolverBase.hpp"

TEST(optic_plan_solver, generate_plan_good)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_optic_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_test.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_test1.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::OPTICPlanSolver>();
  planner->configure(node, "OPTIC");

  auto plan = planner->getPlan(domain_str, problem_str, "generate_plan_good");

  ASSERT_TRUE(plan);
  ASSERT_EQ(plan.value().size(), 4);
  ASSERT_EQ(plan.value()[0].action, "(move_to robot1 room1 room2)");
  ASSERT_EQ(plan.value()[1].action, "(take_object robot1 ball room2)");
  ASSERT_EQ(plan.value()[2].action, "(move_to robot1 room2 room1)");
  ASSERT_EQ(plan.value()[3].action, "(drop_object robot1 ball room1)");
}

TEST(optic_plan_solver, load_optic_plugin)
{
  try {
    pluginlib::ClassLoader<plansys2::PlanSolverBase> lp_loader(
      "plansys2_core", "plansys2::PlanSolverBase");
    plansys2::PlanSolverBase::Ptr plugin =
      lp_loader.createUniqueInstance("plansys2/OPTICPlanSolver");
    ASSERT_TRUE(true);
  } catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(optic_plan_solver, check_ok_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_optic_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_test.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::OPTICPlanSolver>();
  planner->configure(node, "OPTIC");

  auto result = planner->check_domain(domain_str, "check_ok_domain");

  ASSERT_TRUE(result.empty());
}

TEST(optic_plan_solver, check_error_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_optic_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_test_error.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::OPTICPlanSolver>();
  planner->configure(node, "OPTIC");

  auto result = planner->check_domain(domain_str, "check_error_domain");

  ASSERT_FALSE(result.empty());
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);

  return RUN_ALL_TESTS();
}
