# sistemas-de-operacion-abril-julio-2025-proyecto-2

## Descripcion

Este proyecto consiste en el desarrollo de un programa por CLI en lenguaje C, llamdo `dicotodir`. El objetivo principal es crear una estructura de directorios basada en una clave dicotómica para la clasificación de especies botánicas. La clave dicotómica se representa mediante un archivo JSON que contiene una serie de preguntas y respuestas binarias (sí/no) que permiten identificar cada especie.

El programa procesa este archivo JSON para generar una jerarquía de carpetas que refleja las características de cada especie según las respuestas a las preguntas. Esta estructura facilita la organización y búsqueda de especies en función de sus atributos.

Además, como mejora opcional para obtener los créditos adicionales, implementamos la capacidad de utilizar múltiples procesos para acelerar la creación de la estructura de directorios, aprovechando el paralelismo del sistema operativo. `-m --multi`

Este proyecto busca aplicar buenas prácticas de programación en C bajo un entorno Unix, enfatizando la modularidad, el manejo eficiente de memoria y la correcta gestión de procesos y buenas practicas como la organizacion en capaz, aplicación de principios SOLID y Clean Code (intentamos guiarnos más por un Screaming architecture para el scaffolding de los archivos y carpetas).

## Estructura y Diseño

### Organización del Proyecto

El proyecto está organizado siguiendo una arquitectura modular y basada en capas, lo que facilita la separación de responsabilidades y el mantenimiento del código. La estructura de carpetas es la siguiente:

```bash
$ tree
include
├── common
│   ├── errors.h
│   ├── logger.h
│   ├── types.h
│   ├── utils.h
src
├── adapters                                # Adaptadores para interactuar con sistemas externos
│   ├── file_system                         # Manejo del sistema de archivos
│   │   ├── unix_file_system.c
│   │   └── unix_file_system.h
│   └── parsers                             # Parser JSON
│       ├── json_parser.c
│       └── json_parser.h
├── common                                  # Utilidades comunes (logger, errores, etc.)
│   ├── errors.c
│   ├── logger.c
│   └── utils.c
├── core                                    # Lógica central del dominio
│   ├── domain                              # Entidades principales (árbol dicotómico, especies)
│   │   ├── dicotomic_tree.c
│   │   ├── dicotomic_tree.h
│   │   ├── species.c
│   │   └── species.h
│   ├── ports                               # Interfaces para adaptadores
│   │   ├── file_system_port.h
│   │   └── json_parser_port.h
│   └── usecases                            # Casos de uso principales
│       ├── create_directory_structure.c
│       └── create_directory_structure.h
├── infrastructure                          # Implementaciones específicas (CLI, procesos)
│   ├── cli                                 # Parser de argumentos
│   │   ├── args_parser.c
│   │   └── args_parser.h
│   └── process                             # Manejo de procesos
│       ├── process_manager.c
│       └── process_manager.h
└── main.c                                  # Punto de entrada del programa
```

Esta estructura sigue principios de Clean Architecture, donde:

- **Core:** contiene la lógica del dominio y es independiente de los detalles de implementación.
- **Adapters y Infrastructure:** implementan detalles específicos (como el parser JSON o el manejo de procesos).
- **Common:** proporciona utilidades reutilizables, como el logger y manejo de errores.

### Módulos Principales

#### Core (Dominio)

- **DicotomicTree** (`dicotomic_tree.c/h`): Representa el árbol dicotómico, que contiene especies y preguntas únicas. Proporciona funciones para:
  - Crear y liberar árboles (`dicotomic_tree_create`, `dicotomic_tree_free`).
  - Agregar especies al árbol (dicotomic_tree_add_species).
  - Extraer preguntas únicas de las especies (`dicotomic_tree_extract_questions`).
  - Validar que las especies sigan el orden esperado de preguntas (`dicotomic_tree_validate`).
- **Species** (`species.c/h`): Representa una especie con sus características (preguntas y respuestas). Incluye funciones para:
  - Crear y liberar especies (`species_create`, `species_free`).
  - Agregar características a una especie (species_add_characteristic).
  - Validar el orden de las preguntas en una especie (`species_follows_question_order`).

#### Adapters

- **JSON Parser** (`json_parser.c/h`): Implementa el parser JSON utilizando la biblioteca cJSON. Convierte un archivo JSON en un árbol dicotómico (`DicotomicTree`), manejando errores y validando la estructura del archivo.
- **File System** (`unix_file_system.c/h`): Proporciona funciones para interactuar con el sistema de archivos en Unix, como la creación de directorios.

#### Infrastructure

- **CLI** (`args_parser.c/h`): Maneja los argumentos de línea de comandos, permitiendo configurar:

  - Directorio raíz para la estructura de directorios.
  - Textos para respuestas "true" y "false".
  - Modo de concatenación (prefijo, sufijo o ambos).
  - Uso de multiprocesos.

- **Process Manager** (`process_manager.c/h`): Implementa el manejo de procesos en Unix, permitiendo:
  - Crear procesos hijos (`create_child_process`).
  - Esperar a que los procesos hijos terminen (`wait_for_child_processes`).
  - Determinar el número máximo de procesos disponibles (`get_max_processes`).

#### Common

- **Logger** (`logger.c/h`): Proporciona un sistema de logging con niveles (`INFO`, `WARNING`, `ERROR`) y soporte para colores en la terminal.
- **Utils** (`utils.c/h`): Funciones auxiliares como duplicación de cadenas (my_strdup).

### Principios de Diseño

El diseño del proyecto sigue varios principios:

1. **Modularidad:** Cada módulo tiene una responsabilidad clara y está desacoplado de otros módulos. Por ejemplo, el parser JSON no depende de cómo se maneja el sistema de archivos.
2. **Separación de responsabilidades:** La lógica del dominio (árbol dicotómico, especies) está separada de los detalles de implementación (parser JSON, CLI, procesos).Reutilización: Componentes como el logger y las utilidades son genéricos y reutilizables en otros proyectos.
3. **Extensibilidad:** La arquitectura permite agregar nuevos adaptadores (por ejemplo, un parser XML) o cambiar implementaciones (como usar otro sistema de archivos) sin modificar la lógica central.
4. **Uso de estándares Unix:** El manejo de procesos y el sistema de archivos sigue prácticas estándar de programación en Unix.

## Implementación

### Funcionalidades principales

El programa dicotodir implementa las siguientes funcionalidades principales:

1. **Parser de argumentos CLI**: Permite configurar el comportamiento del programa mediante flags y opciones.
2. **Parser JSON**: Convierte un archivo JSON en un árbol dicotómico (`DicotomicTree`), validando la estructura y manejando errores.
3. **Creación de directorios**: Genera una estructura de directorios basada en las preguntas y respuestas del árbol dicotómico.
4. **Multiprocesos**: Optimiza la creación de directorios utilizando procesos hijos en sistemas Unix.
5. **Logger**: Proporciona un sistema de logging con niveles (`INFO`, `WARNING`, `ERROR`) y colores para facilitar la depuración.

### Parser de argumentos CLI

El parser de argumentos (`args_parser.c`) utiliza la biblioteca getopt_long para manejar opciones de línea de comandos. Las opciones permiten configurar:

- Archivo JSON: Especifica el archivo de entrada con la clave dicotómica.
- Directorio raíz: Define dónde se creará la estructura de directorios (`-d` o `--dir`).
- Textos para respuestas: Configura los textos para respuestas "true" y "false" (`-t` y `-f`).
- Modo de concatenación: Permite usar prefijos, sufijos o ambos (`-p` y `-s`).
- Multiprocesos: Activa el uso de procesos hijos para optimizar la creación de directorios (`-m`).

Ejemplo de uso:

```bash
./bin/dicotodir ./input_files/arboles_templados.json -d /tmp/arboles -t "tiene" -f "no tiene" -p -m
```

### Parser JSON

El parser JSON (`json_parser.c`) utiliza la biblioteca cJSON para convertir un archivo JSON en un árbol dicotómico (`DicotomicTree`). Este módulo:

- Valida que el archivo JSON tenga la estructura esperada.
- Extrae las especies y sus características.
- Maneja errores como claves faltantes o valores inválidos.

Ejemplo de entrada JSON:

```json
{
  "Arboles templados": [
    {
      "Pino": [
        { "Hojas como agujas": true },
        { "Hojas de agujas vienen en ramales": true }
      ]
    },
    {
      "Abeto": [
        { "Hojas como agujas": true },
        { "Hojas de agujas vienen en ramales": false }
      ]
    }
  ]
}
```

### Creación de directorios

El módulo `create_directory_structure.c` genera la estructura de directorios basada en las preguntas y respuestas del árbol dicotómico. Las características principales incluyen:

- **Concatenación de textos:** Los nombres de los directorios incluyen las preguntas y respuestas, configurables como prefijos, sufijos o ambos.
- **Creación de archivos de especies:** Cada especie tiene un archivo .txt en su directorio final.
- **Multiprocesos:** Utiliza procesos hijos para paralelizar la creación de directorios.

Ejemplo de estructura generada:

```bash
$ tree /tmp/arboles/Arboles\ templados
/tmp/arboles/Arboles templados
├── no tiene Hojas como agujas
│   ├── no tiene Hojas compuestas
│   │   ├── no tiene Bordes de hoja aserrados
│   │   │   ├── no tiene Bordes de hoja con lobulos
│   │   │   │   └── Magnolia.txt
│   │   │   └── si tiene Bordes de hoja con lobulos
│   │   │       └── Roble Blanco.txt
│   │   └── si tiene Bordes de hoja aserrados
│   │       ├── no tiene Bordes de hoja con espinas
│   │       │   └── Olmo.txt
│   │       └── si tiene Bordes de hoja con espinas
│   │           └── Acebo.txt
│   └── si tiene Hojas compuestas
│       ├── no tiene Sub-hojas parten de un punto central
│       │   └── Nogal.txt
│       └── si tiene Sub-hojas parten de un punto central
│           └── Castanna.txt
└── si tiene Hojas como agujas
    ├── no tiene Hojas de agujas vienen en ramales
    │   └── Abeto.txt
    └── si tiene Hojas de agujas vienen en ramales
        └── Pino.txt

15 directories, 8 files
```

### Multiprocesos

El módulo process_manager.c implementa el manejo de procesos en Unix:

- **Creación de procesos hijos:** Usa `fork()` para dividir el trabajo.
- **Sincronización:** Usa `wait()` para esperar a que los procesos hijos terminen.
- **Optimización:** Limita el número de procesos activos al número de núcleos disponibles (`sysconf(_SC_NPROCESSORS_ONLN)`).

Esto mejora el rendimiento en sistemas con múltiples núcleos.

### Logger

El logger (`logger.c`) proporciona un sistema de logging con niveles y colores:

- **Niveles:** `DEBUG`, `INFO`, `WARNING`, `ERROR`.
- **Colores:** Usa secuencias ANSI para mostrar mensajes en diferentes colores (`INFO` en verde, `WARNING` en amarillo, `ERROR` en rojo).
- **Formato:** Incluye timestamp, nivel y mensaje.

Ejemplo de salida:

```bash
[2025-07-17 12:00:00] [INFO] Created process 12345 for species Pino
[2025-07-17 12:00:01] [ERROR] Failed to create directory for species Abeto
```

### Manejo de memoria y errores

El programa utiliza memoria dinámica para manejar estructuras como DicotomicTree y Species. Se implementaron funciones específicas para liberar memoria y evitar fugas:

- `dicotomic_tree_free`: Libera el árbol dicotómico y sus especies.
- `species_free`: Libera una especie y sus características.

Además, se manejan errores en cada módulo, devolviendo códigos de estado (`StatusCode`) para indicar éxito o fallos específicos.

## Opinión sobre la estructura propuesta (vs. navegación de la interfaz de JavaScript de GoBotany)

En relación con la estructura propuesta para este proyecto, inicialmente consideramos utilizar la biblioteca externa cJSON para el manejo de archivos JSON, dado que es una herramienta ampliamente utilizada y facilita la interpretación de estos archivos. Pero, debido como no sabia si se podian usar librerías externas, optamos por implementar un parser JSON propio, basado en conocimientos previos adquiridos en El Curso de Algoritmos 2, que maneja correctamente los operadores de cierre como corchetes y llaves.

Desde mi experiencia como desarrollador Fullstack, especialmente trabajando con JavaScript, el manejo de archivos JSON resulta mucho más sencillo y natural, ya que el lenguaje ofrece soporte nativo para objetos y estructuras de datos complejas. Esto hace que la navegación y manipulación de datos JSON en JavaScript sea más directa y eficiente en comparación con la implementación manual en C.

En cuanto a la plataforma GoBotany, estoy seguro de que su implementación es mucho más avanzada, ya que pueden utilizar bases de datos tanto relacionales como no relacionales para almacenar y gestionar la información. Esto implica que el manejo y almacenamiento de datos no se limita únicamente a objetos JSON, sino que se apoya en sistemas robustos que permiten consultas eficientes, escalabilidad y mejor integridad de los datos.

Por lo tanto, aunque la estructura propuesta cumple con los requisitos y funciona correctamente, la experiencia de usuario y la facilidad de desarrollo podrían mejorar significativamente si se utilizara un entorno con soporte nativo para JSON y bases de datos avanzadas, tal como lo hace la interfaz de GoBotany.

## Conclusiones

Este proyecto fue un desafío muy divertido que combinó programación en C con estructuras de datos y diseño modular. Diseñar un sistema para leer un archivo JSON y crear una estructura de carpetas basada en preguntas de sí/no me permitió aplicar principios de diseño como la separación de responsabilidades y la modularidad. Esto resultó en código más limpio y fácil de mantener, así que esta experiencia fue muy valiosa para mis futuros proyectos.

Una de las lecciones más importantes que aprendí fue el uso de `gdb` para depurar. Durante todo el proceso, me encontré con bugs bastante complicados como fallas de segmentación y problemas de memoria, pero pude diagnosticar y resolver estos errores con la ayuda de `gdb`. Esta experiencia mejoró mis habilidades para depurar programas en C y me recordó lo importante que es siempre compilar con símbolos de depuración (`-g`) y monitorear de cerca el flujo del programa. Definitivamente voy a usar gdb en mis proyectos futuros.

Este proyecto también reforzó mi comprensión del manejo dinámico de memoria en C, destacando la importancia de liberar toda la memoria asignada para prevenir fugas de memoria. Por eso, voy a ser más cuidadoso con el manejo de memoria en el futuro.

También pude practicar procesos Unix y usar `fork` y `wait` para crear un sistema multi-proceso que crea directorios en paralelo. Esta experiencia me ayudó a entender cómo usar estas llamadas del sistema de manera efectiva. Finalmente, este proyecto me permitió reflexionar sobre las diferencias entre lenguajes de bajo nivel y de alto nivel. Aunque C me permite manejar mejor los recursos, lenguajes como JavaScript hacen que sea fácil parsear JSON e interactuar con bases de datos, lo que realmente me mostró el valor de las herramientas y librerías en los lenguajes modernos.

En general, siento que este proyecto logró sus objetivos y me enseñó nuevas habilidades mientras reforzaba conceptos importantes de programación en C, depuración y diseño de software. Por eso estoy satisfecho con el resultado de este proyecto.
