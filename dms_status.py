from cereal import messaging
import os, time

sm = messaging.SubMaster(['driverMonitoringState'])

while True:
  sm.update()
  os.system("clear")

  dm = sm['driverMonitoringState']
  alert = str(dm.alertLevel)
  awareness = str(dm.visionPolicyState.awarenessPercent)
  lockout = str(dm.lockout)

  print("=== DRIVER ATTENTION STATUS ===")
  print(f"alertLevel: {alert}")
  print(f"awarenessPercent: {awareness}")
  print(f"lockout: {lockout}")

  if not dm.visionPolicyState.faceDetected:
    print("Status: NO DRIVER DETECTED")
  elif dm.visionPolicyState.faceDetected and not dm.visionPolicyState.isDistracted:
    print("Status: DRIVER ATTENTIVE")
  else:
    print("Status: DRIVER DISTRACTED")

  print()
  print(dm)

  time.sleep(0.1)
