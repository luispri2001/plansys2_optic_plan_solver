(define (domain simple_test_error)

(:requirements :strips :typing :durative-actions)

(:types
  robot
  room
  object
)

(:predicates
  (robot_at ?r - robot ?ro - room)
  (object_at ?o - object ?ro - room)
  (robot_arm_available ?r - robot)
  (robot_arm_busy ?r - robot)
)

(:durative-action move_to
    :parameters (?r - robot ?from - room ?to - room)
    :duration (= ?duration 3)
    :condition (and 
        (at start (and
          (robot_at ?r ?from)
        ))
    )
    :effect (and 
        (at start (and
          (not (robot_at ?r ?from))
        ))
        (at end (and 
          (robot_at ?r ?to)
        ))
    )
)

(:durative-action take_object
    :parameters (?r - robot ?o - object ?ro - room
    :duration (= ?duration 1)
    :condition (and 
        (at start (and
          (robot_at ?r ?ro)
          (object_at ?o ?ro)
          (robot_arm_available ?r)
        ))
    )
    :effect (and 
        (at start (and
          (not (object_at ?o ?ro))
          (not (robot_arm_available ?r))
        ))
        (at end (and
          (robot_arm_busy ?r)
        ))
    )
)

(:durative-action drop_object
    :parameters (?r - robot ?o - object ?ro - room)
    :duration (= ?duration 1)
    :condition (and 
        (at start (and
          (robot_at ?r ?ro)
          (robot_arm_busy ?r)
        ))
    )
    :effect (and 
        (at start (and
          (not (robot_arm_busy ?r))
        ))
        (at end (and
          (object_at ?o ?ro)
          (robot_arm_available ?r)
        ))
    )
)
)