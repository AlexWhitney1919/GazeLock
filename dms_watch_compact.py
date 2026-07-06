from cereal import messaging
import time

sm = messaging.SubMaster(['driverStateV2'])

while True:
  sm.update(1000)
  d = sm['driverStateV2']
  print(d)
  time.sleep(0.5)

