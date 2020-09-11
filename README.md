# Tarea2_SEMBII_IsaacVazquez_EnriqueTeran
Tarea 2 de la clase de Sistemas Embebidos 2


Se agrego a la estructura de las tareas un elemento para asignarles su prioridad ya que es estatica,  asi mismo en rtosActivateWaitingTask agregamos la condicional que checa una variable global la cual se actualiza con el valor de la prioridad de la tarea que se esta ejecutando y si esta es mayor a la actual manda la tarea a waiting si es de prioridad mas alta lo pone en ready para ejecutarla.  

Nos surgio un error al no saber por que se llamaba la idle task despues de ejecutar una vez cada tarea , ya que en ninguna parte se hace el llamada y la prioridad de la idle task nunca sera menor a cualquiera de las otras 3 tareas
