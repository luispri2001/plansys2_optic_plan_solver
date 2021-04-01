# Optic Plan Solver
Plan solver for [ROS2 Planning System](https://github.com/IntelligentRoboticsLabs/ros2_planning_system) that uses [OPTIC](https://nms.kcl.ac.uk/planning/software/optic.html) for solving PDDL plans.

## Dependencies
Download and compile OPTIC, for more information visit [OPTIC Homepage](https://nms.kcl.ac.uk/planning/software/optic.html).

To execute [OPTIC](https://nms.kcl.ac.uk/planning/software/optic.html) from any path you must create the following symbolic link where "optic_path" is the path where your executable is located.
~~~
ln -s optic_path optic
~~~

Clone ROS2 Planning System in your ROS2 workspace. For more information [plansys2](https://github.com/IntelligentRoboticsLabs/ros2_planning_system).

## Installation
Clone this repository in your ROS2 workspace
~~~
git clone https://github.com/jmrtzma/plansys2_optic_plan_solver
~~~
Add to file **ros2_planning_system/plansys2_bringup/params/plansys2_params.yaml** the OPTIC plugin like this
~~~
planner:
  ros__parameters:
    plan_solver_plugins: ["OPTIC"]
    POPF:
      plugin: "plansys2/POPFPlanSolver"
    TFD:
      plugin: "plansys2::TFDPlanSolver"
    OPTIC:
      plugin: "plansys2/OPTICPlanSolver"
~~~

## PDDL Support

| Requirements | Supported? |
| ------------ | ----------- |
| **PDDL 1.2** |  |
| :strips | Yes |
| :typing | Yes |
| :disjunctive-preconditions | No |
| :equality | Yes |
| :existential-preconditions | No |
| :universal-preconditions | Yes |
| :conditional-effects | No |
| :domain-axioms | No |
| :subgoals-through-axioms | No |
| :safety-constraints	| No |
| :open-world | No |
| :quantified-preconditions | No |
| :adl | No |
| :ucpop | No |
| **PDDL 2.1** | |
| :numeric-fluents | Yes |
| :durative-actions | Yes |
| :duration-inequalities | Yes |
| :continuous-effects | Yes |
| :negative-preconditions | No |
| **PDDL 2.2** | |
| :derived-predicates | No |
| :timed-initial-literals | Yes |
| **PDDL 3** | |
| :constraints | Yes |
| :preferences | Yes |
| **PDDL 3.1** | |
| :action-costs | Yes |
| :goal-utilities | No |
| **PDDL+** | |
| :time | No |

## Contributors
- Carlos Caminero Abad [@Carlosalpha1](https://github.com/Carlosalpha1)
- Paula Martínez Pérez [@paulamartinezperez](https://github.com/paulamartinezperez)
- María Marcos Hidalgo [@mariamhidalgo](https://github.com/mariamhidalgo)
- Javier Martínez Madruga [@jmrtzma](https://github.com/jmrtzma)