// === TABLERO Y JUEGO BÁSICO ===

// ✅ Inicializar tablero: la función 'inicializarTablero' llena toda la matriz con espacios vacíos.
// Esto permite comenzar una nueva partida sin residuos de jugadas anteriores.

// ✅ Mostrar menú: 'mostrarMenu' muestra las opciones (PvP o PvE) y valida la entrada del usuario.

// ✅ Selección de fichas: 'seleccionPiezas' permite al jugador 1 elegir X u O, y asigna la opuesta al jugador 2.

// ✅ Alternar turnos PvP: en 'jugarPvP', se usa 'turno % 2' para alternar entre los jugadores correctamente.

// ✅ Mostrar tablero en cada turno: 'mostrarTablero' imprime la grilla del Ta-Te-Ti con formato limpio.

// === VALIDACIONES Y ESTADOS DEL JUEGO ===

// 🟡 Validar jugadas: falta implementar la función 'movimientoInvalido'.
// Esta debe verificar si la fila/columna están dentro de rango y si la celda está vacía.

// ✅ Detectar ganador: 'hayGanador' evalúa filas, columnas y diagonales para identificar si hay 3 iguales.

// 🟡 Detectar empate: falta crear la función 'empate', que retorna verdadero si el tablero está lleno y no hay ganador.

// === FUNCIONALIDADES ADICIONALES ===

// 🔴 Modo Jugador vs Máquina: aún no se implementa la función para jugar contra la computadora.
// Se necesita una lógica simple para que la máquina elija una celda válida automáticamente.

// 🟡 Preguntar si jugar de nuevo o salir: después de cada partida, falta agregar una opción para reiniciar o salir del juego.

// ✅ Modularidad general: el programa está bien estructurado en funciones separadas y claras.
// Esto facilita la lectura, mantenimiento y ampliación del código.
