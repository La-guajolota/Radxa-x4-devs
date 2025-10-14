#!/usr/bin/env python3
"""
Servidor Modbus TCP de simulación para pruebas con cliente en C (libmodbus).
Expone 100 holding registers accesibles en la unidad 1.
"""

import asyncio
import logging
from pymodbus.server import StartAsyncTcpServer
from pymodbus.datastore import ModbusServerContext, ModbusSequentialDataBlock
from pymodbus.datastore import ModbusDeviceContext

# Configurar logging para ver operaciones en consola
logging.basicConfig(level=logging.DEBUG)
_logger = logging.getLogger(__name__)


class CallbackDataBlock(ModbusSequentialDataBlock):
    """Bloque de datos con callbacks para debug."""

    def setValues(self, address, value):
        """Al escribir en un registro."""
        super().setValues(address, value)
        _logger.info(f"[WRITE] Dirección={address}, Valor={value}")

    def getValues(self, address, count=1):
        """Al leer registros."""
        result = super().getValues(address, count=count)
        _logger.info(f"[READ] Dirección={address}, Count={count}, Datos={result}")
        return result


async def run_server():
    """Inicia el servidor Modbus TCP en 0.0.0.0:5020."""
    # Creamos 100 holding registers inicializados en 0
    block = CallbackDataBlock(0x00, [0] * 100)

    # Contexto con DI, CO, HR, IR usando el mismo bloque
    store = ModbusDeviceContext(di=block, co=block, hr=block, ir=block)
    context = ModbusServerContext(devices=store, single=True)

    # Levantar servidor en el puerto 5020 (sin framer explícito)
    await StartAsyncTcpServer(
        context=context,
        host="0.0.0.0",
        port=5020,
    )


if __name__ == "__main__":
    asyncio.run(run_server())
