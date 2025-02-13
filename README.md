# TDP1-PetCare
# PetCare: Dispensador de Comida Automático

## Descripción

PetCare es un proyecto desarrollado para automatizar la alimentación de mascotas. El sistema permite programar horarios de dispensado de comida y monitorear el estado del alimento, asegurando que las mascotas reciban su ración diaria de manera puntual y eficiente.

## Índice

1. [Características](#características)
2. [Instalación](#instalación)
3. [Uso](#uso)
4. [Contribución](#contribución)
5. [Licencia](#licencia)
6. [Contacto](#contacto)

## Características

- **Programación de horarios**: Permite establecer múltiples horarios para dispensar comida.
- **Monitoreo de nivel de alimento**: Sensores que detectan la cantidad de comida disponible.
- **Interfaz de usuario**: Aplicación web para configurar y monitorear el dispensador.
- **Notificaciones**: Alertas cuando el nivel de alimento es bajo o en caso de fallos.

## Instalación

### Requisitos previos

- Python 3.8 o superior
- Librerías especificadas en `requirements.txt`

### Pasos de instalación

1. Clonar el repositorio:

   ```bash
   git clone https://github.com/matelarsen/TDP1-PetCare.git
   cd TDP1-PetCare
   ```

2. Crear y activar un entorno virtual:

   ```bash
   python3 -m venv venv
   source venv/bin/activate  # En Windows: venv\Scripts\activate
   ```

3. Instalar las dependencias:

   ```bash
   pip install -r requirements.txt
   ```

4. Configurar las variables necesarias en `config.py`.

5. Ejecutar la aplicación:

   ```bash
   python main.py
   ```

## Uso

Una vez instalada la aplicación, puedes acceder a la interfaz web en `http://localhost:5000`. Desde allí, podrás:

- Configurar los horarios de dispensado.
- Monitorear el nivel de alimento.
- Recibir notificaciones y alertas.

## Contribución

Las contribuciones son bienvenidas. Si deseas colaborar:

1. Haz un fork del repositorio.
2. Crea una nueva rama para tu característica o corrección:

   ```bash
   git checkout -b nueva-caracteristica
   ```

3. Realiza tus cambios y haz commit:

   ```bash
   git commit -m "Descripción de la característica"
   ```

4. Envía tus cambios al repositorio original:

   ```bash
   git push origin nueva-caracteristica
   ```

5. Abre un pull request.

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## Contacto

- **Bruno Zanetti**
- **Franco Niderhaus**
- **Mateo Larsen**

Para más información o consultas, puedes contactarnos a través de nuestros perfiles de GitHub o mediante el correo electrónico proporcionado en la documentación del proyecto.
