#!/usr/bin/python

#  example_kth.py
#  optical_flow_sta
#
#  dataset: http://www.nada.kth.se/cvap/actions/
#  annotations: http://www.umiacs.umd.edu/~zhuolin/PrototypeTree/KTHBoundingBoxInfo.txt
#
#  Created by Srdan Rasic on 5/26/13.

import shlex
from subprocess import Popen, PIPE, STDOUT

actions = ["boxing", "handclapping", "handwaving", "jogging", "running", "walking"]

# annotations file
fh = open("/Users/srdan/Downloads/KTHBoundingBoxInfo.txt","r")

# path to KTH dataset dir (should contain subdirectories named boxing, handclapping, etc...)
kth_dir = "/Users/srdan/Downloads"

# optical flow method
method = "farneback 0.5 1 5 5 5 1.1"
#method = "tvl1 0.25 0.15 0.3 5 5 0.01"


# compute descriptor for each sequence of each action of each person of each scenario
for line in fh.readlines():
  if line.startswith("#") or line == "":
    continue
  
  elements = line.split()
  person = elements[0]
  scenario = elements[1]
  action = int(elements[2])
  sequence = elements[3]
  startFrame = int(elements[4])
  endFrame = int(elements[5])

  filename = "person%02i_%s_d%s_uncomp.avi" % (int(person), actions[action-1],scenario)
  totalFrames = endFrame-startFrame+1
  frameNumber = startFrame

  # read annotations
  annotations = []
  for i in range(6,6+totalFrames*4,4):
    
    ymin = int(elements[i])
    xmin = int(elements[i+1])
    ymax = int(elements[i+2])
    xmax = int(elements[i+3])
  
    annotations.append("{} {} {} {}".format(xmin, ymin, xmax, ymax))

  # command line shell string
  # remove -v when evaluating! (disables visualizations thus improving performance)
  #   add -i to enable interactive mode
  # consult './optical_flow_sta --help' for more info
  #
  command_line = """./optical_flow_sta
    -v
    --input {dir}/{action}/{file}
    --sta_order 2
    --bins_sta1 6 --bins_sta2 6 
    --patches_v 3 --patches_h 3
    --measurement flow
    --flow_method {method}
    --annotations_stdin --range {start} {end}
    """.format(dir=kth_dir, action=actions[action-1], file=filename, method=method, start=startFrame, end=endFrame)

  args = shlex.split(command_line)
  p = Popen(args, stdout=PIPE, stdin=PIPE, stderr=STDOUT)

  # execute (pass annotations through stdin); descriptor is in stdoutdata (0-element of tuple)
  descriptor = p.communicate(input=" ".join(annotations))[0].rstrip()

  identifier = "{}{}{}{}".format(person,action,scenario,sequence)
  print(identifier, descriptor)


