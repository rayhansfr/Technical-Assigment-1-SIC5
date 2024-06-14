from flask import Flask, request, jsonify, render_template
from datetime import datetime

app = Flask(__name__)

# List to store sensor data
sensor_data = []

@app.route('/sensor', methods=['POST'])
def sensor_data_endpoint():
    data = request.get_json()
    if not data:
        return jsonify({"error": "Invalid data"}), 400
    
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    
    if temperature is None or humidity is None:
        return jsonify({"error": "Missing data"}), 400
    
    # Log the received data
    print(f"Received temperature: {temperature}, humidity: {humidity}")
    
    # Save data with timestamp
    sensor_data.append({
        "temperature": temperature,
        "humidity": humidity,
        "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    })

    return jsonify({"status": "success", "data": data}), 200

@app.route('/')
def index():
    return render_template('index.html', data=sensor_data)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
