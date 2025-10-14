#!/usr/bin/env python3
"""
Servidor CoAP que simula la lectura de 3 sensores:
- Sensor de temperatura
- Sensor de humedad  
- Sensor de presión atmosférica

Requiere: pip install aiocoap
"""

import asyncio
import json
import random
import time
from datetime import datetime
import aiocoap.resource as resource
import aiocoap


class TemperatureSensor(resource.Resource):
    """Recurso que simula un sensor de temperatura"""
    
    async def render_get(self, request):
        # Simula temperatura entre 15°C y 35°C
        temperature = round(random.uniform(15.0, 35.0), 2)
        timestamp = datetime.now().isoformat()
        
        data = {
            "sensor": "temperature",
            "value": temperature,
            "unit": "°C",
            "timestamp": timestamp,
            "status": "active"
        }
        
        payload = json.dumps(data, indent=2).encode('utf-8')
        return aiocoap.Message(payload=payload, content_format=0)


class HumiditySensor(resource.Resource):
    """Recurso que simula un sensor de humedad"""
    
    async def render_get(self, request):
        # Simula humedad entre 30% y 90%
        humidity = round(random.uniform(30.0, 90.0), 1)
        timestamp = datetime.now().isoformat()
        
        data = {
            "sensor": "humidity",
            "value": humidity,
            "unit": "%",
            "timestamp": timestamp,
            "status": "active"
        }
        
        payload = json.dumps(data, indent=2).encode('utf-8')
        return aiocoap.Message(payload=payload, content_format=0)


class PressureSensor(resource.Resource):
    """Recurso que simula un sensor de presión atmosférica"""
    
    async def render_get(self, request):
        # Simula presión entre 950 hPa y 1050 hPa
        pressure = round(random.uniform(950.0, 1050.0), 1)
        timestamp = datetime.now().isoformat()
        
        data = {
            "sensor": "pressure",
            "value": pressure,
            "unit": "hPa",
            "timestamp": timestamp,
            "status": "active"
        }
        
        payload = json.dumps(data, indent=2).encode('utf-8')
        return aiocoap.Message(payload=payload, content_format=0)


class AllSensors(resource.Resource):
    """Recurso que devuelve las lecturas de todos los sensores"""
    
    async def render_get(self, request):
        timestamp = datetime.now().isoformat()
        
        data = {
            "sensors": {
                "temperature": {
                    "value": round(random.uniform(15.0, 35.0), 2),
                    "unit": "°C"
                },
                "humidity": {
                    "value": round(random.uniform(30.0, 90.0), 1),
                    "unit": "%"
                },
                "pressure": {
                    "value": round(random.uniform(950.0, 1050.0), 1),
                    "unit": "hPa"
                }
            },
            "timestamp": timestamp,
            "status": "all_active"
        }
        
        payload = json.dumps(data, indent=2).encode('utf-8')
        return aiocoap.Message(payload=payload, content_format=0)


class ServerInfo(resource.Resource):
    """Recurso con información del servidor"""
    
    async def render_get(self, request):
        data = {
            "server": "CoAP Sensor Simulator",
            "version": "1.0",
            "sensors_available": [
                "/sensors/temperature",
                "/sensors/humidity", 
                "/sensors/pressure",
                "/sensors/all"
            ],
            "description": "Servidor CoAP que simula 3 sensores ambientales",
            "timestamp": datetime.now().isoformat()
        }
        
        payload = json.dumps(data, indent=2).encode('utf-8')
        return aiocoap.Message(payload=payload, content_format=0)


async def main():
    """Función principal que inicia el servidor CoAP"""
    
    # Crear el contexto del servidor
    root = resource.Site()
    
    # Registrar los recursos
    root.add_resource(['.well-known', 'core'], resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(['info'], ServerInfo())
    root.add_resource(['sensors', 'temperature'], TemperatureSensor())
    root.add_resource(['sensors', 'humidity'], HumiditySensor())
    root.add_resource(['sensors', 'pressure'], PressureSensor())
    root.add_resource(['sensors', 'all'], AllSensors())
    
    # Crear y configurar el contexto del servidor
    # Bind a todas las interfaces (0.0.0.0) para permitir conexiones remotas
    await aiocoap.Context.create_server_context(root, bind=('0.0.0.0', 5683))
    
    print("🌡️  Servidor CoAP iniciado en todas las interfaces (0.0.0.0:5683)")
    print("\n📊 Recursos disponibles desde la red:")
    print("  - coap://<IP_DEL_SBC>:5683/info")
    print("  - coap://<IP_DEL_SBC>:5683/sensors/temperature")
    print("  - coap://<IP_DEL_SBC>:5683/sensors/humidity")
    print("  - coap://<IP_DEL_SBC>:5683/sensors/pressure")
    print("  - coap://<IP_DEL_SBC>:5683/sensors/all")
    print("\n🔍 Para probar desde otra computadora:")
    print("  coap-client -m get coap://<IP_DEL_SBC>:5683/sensors/temperature")
    print("  coap-client -m get coap://<IP_DEL_SBC>:5683/sensors/all")
    print("\n⚡ Presiona Ctrl+C para detener el servidor\n")
    
    # Mantener el servidor ejecutándose
    try:
        await asyncio.get_running_loop().create_future()
    except KeyboardInterrupt:
        print("\n🛑 Servidor detenido")


if __name__ == "__main__":
    # Ejecutar el servidor
    asyncio.run(main())