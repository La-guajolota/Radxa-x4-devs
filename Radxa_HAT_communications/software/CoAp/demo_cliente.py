#!/usr/bin/env python3
"""
Cliente CoAP para conectarse remotamente al servidor en SBC

Requiere: pip install aiocoap
"""

import asyncio
import json
import socket
import aiocoap


class CoAPSensorClient:
    def __init__(self, sbc_ip, port=5683):
        self.sbc_ip = sbc_ip
        self.port = port
        self.base_uri = f"coap://{sbc_ip}:{port}"
        
    async def get_sensor_data(self, endpoint):
        """Obtiene datos de un endpoint específico"""
        context = await aiocoap.Context.create_client_context()
        
        try:
            uri = f"{self.base_uri}/{endpoint}"
            request = aiocoap.Message(code=aiocoap.GET, uri=uri)
            response = await context.request(request).response
            
            if response.payload:
                data = json.loads(response.payload.decode('utf-8'))
                return data
            else:
                return {"error": "No se recibió respuesta"}
                
        except Exception as e:
            return {"error": f"Error de conexión: {str(e)}"}
        finally:
            await context.shutdown()
    
    async def test_connection(self):
        """Prueba la conectividad con el servidor"""
        print(f"🔗 Probando conexión con SBC en {self.sbc_ip}...")
        
        data = await self.get_sensor_data("info")
        
        if "error" in data:
            print(f"❌ No se pudo conectar: {data['error']}")
            return False
        else:
            print(f"✅ Conexión exitosa!")
            print(f"   Servidor: {data.get('server', 'N/A')}")
            print(f"   Versión: {data.get('version', 'N/A')}")
            return True
    
    async def read_all_sensors(self):
        """Lee todos los sensores una vez"""
        print("\n📊 Leyendo todos los sensores...")
        
        sensors = [
            ("🌡️  Temperatura", "sensors/temperature"),
            ("💧 Humedad", "sensors/humidity"),
            ("🔽 Presión", "sensors/pressure")
        ]
        
        for name, endpoint in sensors:
            data = await self.get_sensor_data(endpoint)
            
            if "error" not in data:
                value = data.get('value', 'N/A')
                unit = data.get('unit', '')
                timestamp = data.get('timestamp', '').split('T')[1][:8]  # Solo hora
                print(f"   {name}: {value}{unit} [{timestamp}]")
            else:
                print(f"   {name}: ❌ {data['error']}")
    
    async def continuous_monitoring(self, interval=3):
        """Monitoreo continuo de sensores"""
        print(f"\n🔄 Iniciando monitoreo continuo (cada {interval}s)...")
        print("   Presiona Ctrl+C para detener\n")
        
        try:
            while True:
                data = await self.get_sensor_data("sensors/all")
                
                if "error" not in data:
                    sensors = data.get("sensors", {})
                    timestamp = data.get("timestamp", "").split('T')[1][:8]
                    
                    temp = sensors.get('temperature', {}).get('value', 'N/A')
                    hum = sensors.get('humidity', {}).get('value', 'N/A')
                    pres = sensors.get('pressure', {}).get('value', 'N/A')
                    
                    print(f"[{timestamp}] 🌡️ {temp}°C | 💧 {hum}% | 🔽 {pres} hPa")
                else:
                    print(f"❌ Error: {data['error']}")
                
                await asyncio.sleep(interval)
                
        except KeyboardInterrupt:
            print("\n🛑 Monitoreo detenido")


def discover_sbc_ip():
    """Intenta descubrir automáticamente la IP del SBC en la red local"""
    print("🔍 Buscando SBCs en la red local...")
    
    # Obtener IP de la computadora local
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()
    
    network = ".".join(local_ip.split(".")[:-1])
    print(f"   Red local detectada: {network}.0/24")
    
    # IPs comunes para SBCs
    common_ips = [
        f"{network}.100",  # Raspberry Pi típica
        f"{network}.101",
        f"{network}.10",
        f"{network}.20"
    ]
    
    print("   Probando IPs comunes para SBCs...")
    for ip in common_ips:
        try:
            # Ping rápido
            response = socket.create_connection((ip, 5683), timeout=1)
            response.close()
            print(f"   ✅ Encontrado servidor en {ip}")
            return ip
        except:
            continue
    
    print("   ❌ No se encontró automáticamente")
    return None


async def main():
    """Función principal del cliente remoto"""
    
    print("🤖 Cliente CoAP Remoto (PC → SBC)")
    print("=" * 50)
    
    # Intentar descubrir la IP del SBC
    sbc_ip = discover_sbc_ip()
    
    if not sbc_ip:
        print("\n📝 Ingresa manualmente la información del SBC:")
        sbc_ip = input("   IP del SBC: ").strip()
        
        if not sbc_ip:
            print("❌ IP requerida")
            return
    
    # Crear cliente
    client = CoAPSensorClient(sbc_ip)
    
    # Probar conexión
    if not await client.test_connection():
        print("\n💡 Sugerencias:")
        print("   - Verifica que el servidor esté ejecutándose en el SBC")
        print("   - Confirma que la IP sea correcta")
        print("   - Asegúrate de que el puerto 5683 esté abierto")
        return
    
    # Menú de opciones
    while True:
        print("\n" + "=" * 50)
        print("1. Lectura única de todos los sensores")
        print("2. Monitoreo continuo")
        print("3. Cambiar IP del SBC")
        print("4. Salir")
        print("=" * 50)
        
        try:
            choice = input("Selecciona una opción: ").strip()
            
            if choice == "1":
                await client.read_all_sensors()
                
            elif choice == "2":
                interval = input("Intervalo en segundos (3): ").strip()
                interval = int(interval) if interval.isdigit() else 3
                await client.continuous_monitoring(interval)
                
            elif choice == "3":
                new_ip = input("Nueva IP del SBC: ").strip()
                if new_ip:
                    client = CoAPSensorClient(new_ip)
                    await client.test_connection()
                    
            elif choice == "4":
                break
                
            else:
                print("❌ Opción no válida")
                
        except KeyboardInterrupt:
            print("\n🛑 Cliente terminado")
            break
        except ValueError:
            print("❌ Valor inválido")


if __name__ == "__main__":
    asyncio.run(main())