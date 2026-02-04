import paho.mqtt.client as mqtt
import json
import datetime

mqttbroker = "127.0.0.1"
jsonf = 'students.json'
escapedist = 4.5 # distance to door in meters
self_id = "iBeacon:e5ca1ade-f007-ba11-0000-000000000000-148-53065"
present_students = set()

def on_message(client, userdata, msg):
    device_id = msg.topic.split('/')[-1]
    # prevents self-detection loop
    if device_id == self_id:
        return
    try:
        # checks database for known students
        with open(jsonf, 'r') as f: db = json.load(f)
        
        if device_id in db:
            student_name = db[device_id]
            data = json.loads(msg.payload.decode())
            dist = data.get("distance", 99)

            if dist < escapedist:
                if student_name not in present_students:
                    print(f"{student_name} entered the room.")
                    present_students.add(student_name)
            else:
                if student_name in present_students:
                    timestamp = datetime.datetime.now().strftime("%H:%M:%S")
                    print(f"{student_name} left at {timestamp}!")
                    present_students.remove(student_name)
    except: pass

client = mqtt.Client()
client.on_message = on_message

print(f"Connecting to broker at {mqttbroker}...")
client.connect(mqttbroker, 1883)
client.subscribe("espresense/devices/#")

print("Monitoring presense (Ctrl+C to stop).")
client.loop_forever()