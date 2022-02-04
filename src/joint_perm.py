from re import search
import numpy as np # Scientific computing library for Python
import math

PI = math.pi

# 1 - 10
JOINT6_CNT = 1
JOINT4_CNT = 1
SPLIT = 10

JOINT1_MIN = (int) (-PI * 180/PI)
JOINT1_MAX = (int) (PI * 180/PI)

JOINT2_MIN = (int) (-2.0 * 180/PI)
JOINT2_MAX = (int) (2.0 * 180/PI)

JOINT3_MIN = (int) (-PI * 180/PI)
JOINT3_MAX = (int) (PI/2 * 180/PI)

JOINT4_MIN = (int) (-0.875*PI * 180/PI)
JOINT4_MAX = (int) (0.875*PI * 180/PI)
JOINT4_START = (int) ((JOINT4_MAX - JOINT4_MIN) / SPLIT * (JOINT4_CNT-1)) + JOINT4_MIN
JOINT4_END = (int) ((JOINT4_MAX - JOINT4_MIN) / SPLIT * JOINT4_CNT) + JOINT4_MIN 

JOINT5_MIN = (int) (-PI/2 * 180/PI)
JOINT5_MAX = (int) (PI/2 * 180/PI)

JOINT6_MIN = (int) (-PI * 180/PI)
JOINT6_MAX = (int) (PI * 180/PI)
JOINT6_START = (int) ((JOINT6_MAX - JOINT6_MIN) / SPLIT * (JOINT6_CNT-1)) + JOINT6_MIN
JOINT6_END = (int) ((JOINT6_MAX - JOINT6_MIN) / SPLIT * JOINT6_CNT) + JOINT6_MIN 

RES = 5
# LIMIT = 100000

print("Joint 4 Min/Max: ", JOINT4_MIN, JOINT4_MAX)
print("Joint 4 Start/End: ", JOINT4_START, JOINT4_END)
print("Joint 6 Min/Max: ", JOINT6_MIN, JOINT6_MAX)
print("Joint 6 Start/End: ", JOINT6_START, JOINT6_END)

def generate_search_space():
  textfile = open("jointsearchspace5_split1of100.txt", "w")
  append_count = 0
  for joint6 in range(JOINT6_START,JOINT6_END+1, RES):
    for joint5 in range(JOINT5_MIN,JOINT5_MAX+1, RES):
      for joint4 in range(JOINT4_START,JOINT4_END+1,RES):
        print([joint4, joint5, joint6])
        for joint3 in range(JOINT3_MIN,JOINT3_MAX+1,RES):
          for joint2 in range(JOINT2_MIN,JOINT2_MAX+1,RES):
            for joint1 in range(JOINT1_MIN,JOINT1_MAX+1,RES):

              # append_count+=1
              # if append_count > LIMIT:
              #   textfile.close()
              #   return 

              joint_values = [joint1 * PI/180, 
                              joint2 * PI/180, 
                              joint3 * PI/180, 
                              joint4 * PI/180, 
                              joint5 * PI/180, 
                              joint6 * PI/180
              ]
              for joint in joint_values:
                textfile.write(str(joint)+"   ")
              textfile.write("\n")        
  
generate_search_space()