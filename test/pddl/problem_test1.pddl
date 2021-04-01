(define (problem simple_test1) 
(:domain simple_test)
(:objects
  room1 room2 - room
  robot1 - robot
  ball - object
)

(:init
  (robot_at robot1 room1)
  (robot_arm_available robot1)
  (object_at ball room2)
)

(:goal (and
  (robot_at robot1 room1)
  (object_at ball room1)
))
)
