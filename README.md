# ESTRUCTURAS DE DATOS Y ALGORITMOS II, 2022-2023

<p style="text-align:center;">Javier Bellón Cordobés, David Cobler Gallego, Dídac Casacuberta Santamaría <br>
266933, 268693, 268698 <br>
11/06/2023
</p>

## INTRODUCCIÓN

<p>Para el proyecto de la asignatura hemos hecho como el resto de grupos una red social. En nuestro caso no hemos hecho nada que diferencie a nuestra red social de las demás, al menos en cuanto a metodología, nos hemos centrado en seguir los objetivos, tanto obligatorios como opcionales para obtener el resultado deseado. </p>

<p>Lo que haría a nuestra red social diferente del resto sería la diferencia en cuanto a la experiencia de uso de nuestra red social, esto se debe a que mediante la API de Windows hemos creado una interfaz gráfica sobre la que interactuar, haciendo que no sea necesaria en absoluto la interfaz de comandos. Por el camino hemos encontrado muchos problemas que comentaremos cuando hablemos individualmente de las diferentes partes del proyecto y nos habría gustado haber hecho más cosas pero hemos tenido que conformarnos con el resultado final que hemos entregado, que funciona perfectamente, antes que intentar agregar más funcionalidades que posiblemente rompan la aplicación.</p>

<p>Es importante aclarar que hemos desarrollado en paralelo dos versiones diferentes del proyecto, una para interfaz gráfica y otra para consola. Empezamos haciendo esto para poder tener un respaldo en caso de encontrarnos que fuera muy difícil hacerlo con interfaz gráfica fuera muy difícil y conforme la curva de dificultad se fue reduciendo acabamos usando la versión de terminal principalmente como prototipo para implementar funcionalidades ahí más fácilmente y luego pasarlas a la interfaz gráfica.</p>

<p>Para compilar correctamente la aplicación es necesario utilizar un PC con sistema operativo Windows e instalar previamente la SDK de Windows, la cual se puede descargar desde el siguiente enlace:</p>

<a href="https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/">https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/</a>

## OBJETIVOS DEL PROYECTO



<p>Antes de empezar con la explicación detallada de los objetivos del proyecto queremos aclarar algo que se va a ver mucho conforme expliquemos los puntos y adjuntemos capturas. En la versión de interfaz gráfica que hemos hecho, que es el proyecto principal, no hemos utilizado caracteres o strings de tipo <i>char</i>, sino de tipo <i>wchar_t</i>, la diferencia principal es que mientras los <i>char</i> están codificados en ASCII, los <i>wchar_t</i> están codificados en UTF-16LE (Little Endian) que es la codificación nativa de windows. Este cambio lo hemos hecho para poder representar caracteres que el conjunto ASCII no puede representar como vocales con tildes por poner un ejemplo, ya que vimos que las librerías de WINAPI eran perfectamente compatibles con esta codificación.</p>

<p>Por lo tanto las funciones que normalmente tratan strings tienen un nombre diferente porque es su equivalente para <i>wchar_t</i>, un ejemplo puede ser <i>strcpy</i> que pasa a ser <i>wcscpy</i>. Asimismo la forma de escribir una string de <i>wchar_t</i> cambia añadiéndole una L delante a los caracteres delimitados por comillas.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/4587dc2f-fda4-4baa-bb62-726c8dde78cc" alt='L"Información"'/></p>

<p>También queremos destacar que hemos tenido que modificar CMakeLists.txt para poder ejecutar el programa. Con esta línea podemos ejecutar aplicaciones con la API de Windows, con unicode y con output por consola respectivamente.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/994cf20b-98ff-4c2b-b305-c952e42e17ed" alt='set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -municode -mconsole"'/>
</p>

<p>Hemos tenido muchos problemas con el cmake-build-debug en git, por eso lo hemos excluido del repositorio. Para poder ejecutar el proyecto correctamente después de hacer un pull por primera vez hay que crear en CLion el proyecto de nuevo en el mismo directorio y seleccionar la opción de crear desde archivos existentes.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/56c61a92-bfd8-41f5-94e2-dafbadc3fd91" alt='Create from Existing Sources'/>
</p>

<p>Por último, todos los cambios derivados de programar una interfaz visual se explicarán cuando hablemos del objetivo de la interfaz en detalle, en el resto de objetivos no haremos mucho énfasis en los cambios más que aclarar algunas cosas que no queden claras.</p>

### Objetivos obligatorios alcanzados

<p>Explicaremos los objetivos siguiendo el orden en el que aparecen los objetivos en la guía de prácticas, primero explicaremos nuestra implementación de una o más listas, colas y stacks.
</p>

<p>En la aplicación hemos usado una lista para guardar todos los usuarios registrados.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/1319428d-f43b-4c09-b59e-6783a12298e5" alt='Estructuras UserList y UserNode'/>
</p>

<p>Cuando se carga la aplicación, esta lista se inicializa vacía.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/d696ced8-a0bc-4a6a-bc57-fc7d5836bc98" alt='Inicializando UserList'/>
</p>

<p>Cuando se crean los usuarios se añaden a la lista. Con la función <i>addUser</i> creamos un <i>UserNode</i> en el que metemos el usuario y lo añadimos a la lista, cubriendo así la parte de inserción de datos en la lista
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/73d66d37-6ebf-43f3-8f0d-e2908bb52cd5" alt='función addUser'/>
</p>

<p>Para recoger un usuario de la lista utilizamos la función <i>getUser</i> que a partir de un texto que incluye la id del usuario busca linealmente en la lista y devuelve al usuario, o NULL si no lo encuentra. Recalcamos que el motivo de la estructura del parámetro de entrada usernameid se explicará más detalladamente cuando expliquemos la interfaz de usuario.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/776d306b-be11-4505-ba56-851c8a66ded2" alt='función getUser'/>
</p>

<p>La lista de usuarios fue lo primero que hicimos y tardamos alrededor de una hora en hacer correctamente todos los algoritmos necesarios para gestionarla. Nuestra implementación es muy mejorable y esto se puede notar sobre todo al ver las siguientes estructuras de las que hablaremos en las que habiendo cogido ya práctica hicimos una implementación más limpia y estructurada. Un ejemplo conciso de cómo se podría mejorar es creando una función para inicializar la lista en lugar de hacer la declaración “a mano” al iniciar la aplicación.</p>
<br>

Código relacionado con la lista está en:
* headers/users.h: líneas 48-58
* sources/users.c: líneas 18-54
<br><br>

<p>Seguimos con la pila, el stack que tenemos implementado está completamente ligado a la gestión de los elementos de la interfaz gráfica por lo que tendremos que dejar el “porque” para luego, por ahora explicamos el “como”.</p>

<p>Nuestro Stack tiene una implementación bastante sencilla. La estructura es sencillamente un array y su tamaño.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/2656d26c-4d90-4330-8b9d-9bf448f4af9b" alt='estructura handleStack'/>
</p>

<p>Tenemos una función que devuelve un Stack declarado e inicializado
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/f1362bdf-8d92-4775-a6cb-1ecc5aeb812a" alt='función initHandleStack'/>
</p>

<p>Para agregar elementos al stack, asignamos memoria para acomodar el nuevo elemento en el array y añadimos el nuevo elemento al final.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/99b99ea8-d95a-4d67-a94c-d41faaab66d2" alt='función addHandle'/>
</p>

<p>Con la función <i>popHandle</i> cogemos el último elemento del stack y lo eliminamos a la vez. No hacemos una reasignación de memoria porque sería redundante y podría hacer que el programa fuera más lento, al agregar un nuevo elemento ya se asignará la memoria al tamaño que le toque y se liberaran o sobreescribirán los elementos que el usuario haya eliminado anteriormente.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/db34dae7-9bd1-48a4-9c06-e562969e64a2" alt='función popHandle'/>
</p>

<p>Tardamos unos veinte minutos en implementar toda la lógica perteneciente al stack, una mejoría que podríamos haber hecho es hacer una reasignación de espacio más dinámica en la que el stack siempre crece en tamaño y nunca hace un realloc hacia abajo, esta forma de hacerlo reduciría mucho la cantidad de asignaciones de memoria de un stack cuyo tamaño mínimo nunca baja y siempre puede subir por la forma en la que lo estamos utilizando.</p>
<br>

El código relacionado con el stack está en:
- headers/handleStack.h: 9-12
- sources/handleStack.c: 8-43 (todo el archivo)
<br><br>

<p>Por último, nuestra cola consiste en los posts que los usuarios escriben en su timeline. Cada usuario tiene su propia cola de Posts.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/3a255fe8-1925-407f-bf80-223e1cfe5e4a" alt='estructuras Post y PostQueue'/>
</p>

<p>Cuando se cree un usuario su <i>PostQueue</i> se inicializará usando la función <i>initQueue</i>
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/20067a2e-5f96-4bf7-b352-5472dd24e1fa" alt='función initQueue'/>
</p>

<p>Para agregar posts utilizamos la función <i>addToQueue</i>, que los agrega al final de la cola
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/e8144477-f3ed-45e2-8b4b-8171841d46b3" alt='función addToQueue'/>
</p>

<p>Para sacar elementos de la cola utilizamos la función <i>dequeue</i>. Esta función devuelve el post en la posición actual del puntero de la cola y mueve el puntero una posición adelante. Debido a la forma de implementar los posts en la interfaz que hemos utilizado no borramos elementos de la cola, si no que tenemos un puntero que indica cual es el elemento que debe devolver la cola.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/5ec4a1aa-7fee-424e-b6c3-89697e31b493" alt='función dequeue'/>
</p>

<p>Necesitamos que nuestras colas de posts sean reutilizables por lo tanto tenemos una función <i>resetQueue</i> que reinicia el valor del puntero.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/34fc0f15-80e9-4ab3-b739-429b503d1b36" alt='función resetQueue'/>
</p>

<p>Toda la lógica de la implementación de la cola nos tomó unos cuarenta y cinco minutos. En esta implementación no creemos que se pueda mejorar nada ya que para el uso que le hemos dado es perfecta además, los pequeños problemas que tuvimos anteriormente con la lista y el stack que los hacía mejorables están corregidos en la implementación de la cola.</p>
<br>

El código relacionado con la cola está en:
- headers/users.h: 23-33
- sources/PostQueue.c: 7-47 (todo el archivo)
<br><br>

<p>El siguiente objetivo consiste en utilizar algoritmos de búsqueda. Ya hemos mencionado como en la lista de usuarios buscamos mediante <i>LinearSearch</i> pero podemos poner otro ejemplo en el que utilizamos el mismo algoritmo. 
</p>

<p>Para manejar los amigos y solicitudes de amistad tenemos una estructura que consiste en un array dinámico de usuarios
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/4cdfd75f-3332-4c02-b7a8-8191852a7644" alt='estructura user_array'/>
</p>

<p>Hemos implementado una función llamada <i>search_User</i> para buscar un usuario específico por su id en el array.
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/ebeb43aa-5e28-4f4a-a80c-c2d1e5291dd0" alt='función search_User'/>
</p>

<p>También hemos creado una función para eliminar usuarios del array en la que también buscamos linealmente
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/91db3001-8dea-4b6a-b581-560674eb4b89" alt='función remove_from_array'/>
</p>

<p>Debido a la facilidad de implementación de una búsqueda lineal solo tardamos unos quince minutos. Podríamos haber mejorado la implementación de la búsqueda haciendo un <i>BinarySearch</i> ya que ordenar el array no afecta al funcionamiento de la aplicación y habríamos ganado rendimiento a la hora de buscar usuarios en las listas de amigos o peticiones de amistad. Otro aspecto que podríamos haber mejorado es la utilidad de la función <i>search_User</i>, podría devolver directamente el usuario y así ahorrar la implementación de una segunda búsqueda lineal en <i>remove_from_array</i>.</p>

<br>

Código relacionado:
- headers/users.h: 18-21
- sources/users_arrays.c: 61-93
<br><br>

<p>Pasamos a documentar el siguiente objetivo que son los algoritmos de ordenación, hemos utilizado un <i>MergeSort</i> para ordenar el diccionario para contar palabras que hemos implementado. Ordenar un diccionario es una acción muy destructiva y explicaremos el motivo y como lo hemos gestionado más adelante al hablar más en detalle del diccionario. La estructura que tiene nuestro diccionario es un array dinámico de nodos con clave y valor numérico
<br><img src=https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/42cf7cc8-1c87-4eb9-83b0-0450bed03350"" alt='estructuras DictNode y WDict'/>
</p>

<p>En este caso no vamos a adjuntar captura de todo el <i>MergeSort</i>, nos vamos a centrar un poco en las líneas que adaptan el algoritmo a nuestro código. Al hacer la comparación entre los dos diferentes arrays temporales lo que estamos comparando es el valor Count del nodo y el que sea mayor se aplica antes
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/f7a59768-3487-4a5d-a573-67b914ca170e" alt='while del MergeSort del diccionario'/>
</p>

<p>La implementación del <i>MergeSort</i> nos llevó unas dos horas en total. Al principio tuvimos problemas cómo hacer de forma incorrecta las llamadas recursivas o no hacer bien la comparación del tamaño del count y por la naturaleza de nuestra aplicación y de lo que estamos intentando ordenar se hacía difícil encontrar qué era lo que estaba fallando, pero después de sacarlo creemos que ha merecido la pena. El diccionario normalmente tiene un tamaño muy grande y necesitábamos algo con poca complejidad temporal para no dejar colgada la pantalla al enviar un mensaje por lo que <i>BubbleSort</i> o <i>InsertionSort</i> no servían en absoluto.</p>
<br>

El código relacionado se encuentra en:
- sources/WordDictionary.c: 80-132
<br><br>

<p>Ahora hablamos en detalle del diccionario, la estructura del cual hemos mostrado previamente
<br><img src="https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/a260c62f-22b4-42a8-8269-41db3b0bfdd9" alt='estructuras DictNode y WDict'/>
</p>

Como con las otras estructuras tenemos una función para inicializar el diccionario de nombre _initDict_<br>
![función initDict](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/0d9ac092-bc0a-4fc5-9073-4847bacb0b08)
<br><br>
Las funciones más importantes son sin embargo las que se encargan de buscar y añadir palabras, _searchDict_ y _addToDict_ respectivamente.
<br><br>
La fórmula que hemos seguido para encontrar la clave consiste en convertir cada carácter de la palabra en su valor único, sumar todos los valores y por último multiplicar por el número de caracteres. El valor obtenido lo dividimos después entre el tamaño del diccionario y el residuo será el índice del nuevo conjunto clave-valor.
<br>
![cálculo posición 1](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/c78e6f70-2689-4928-8f77-7270f5778928)
<br>
![cálculo posición 2](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/e9e20f46-9c97-4ddb-a7fc-3ed5ac6953bc)
<br><br>

En caso de colisión, resolvemos el problema mediante sondeo lineal, buscando la siguiente posición vacía (_addToDict_) o la posición cuya clave encaje con la que buscamos (_searchDict_). 
<br>
![colisión función addToDict](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/bd24dbae-cc02-4221-9f95-8a422c810fdf)
<br>
![colisión función searchDict](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/cd0bdf8d-0137-4542-806d-f70a72572431)
<br><br>
Los algoritmos para asignar posición y para resolver colisiones son la parte más importante de la implementación funcional del diccionario y una vez explicadas estas partes vamos a elaborar en qué uso le damos al diccionario.
<br><br>
Cuando el usuario crea un post cogemos el texto y lo partimos en base a saltos de línea, espacios y tabulaciones. Cada palabra se buscará si existe en el diccionario y en caso que sí se le sumará uno al contador, si no existe se crea la entrada para la palabra, que se crea con un contador de uno.
<br>
![Explicación diccionario send post](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/c3b437c8-ce01-49b2-a463-fe1006b0ef4a)
<br><br>
Cuando el usuario quiera visualizar las palabras globales más utilizadas podrá verlo en la información de su perfil (explicación en apartado sobre  la interfaz gráfica). Al cargar la información se creará un diccionario temporal en el que se cargarán todos los datos del original y este diccionario se ordenará con el _MergeSort_ visto anteriormente.
<br>
![Llamada a MergeSort con diccionario temporal](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/00e22c4f-3a02-4502-9648-bb63e465786f)
<br><br>
Cuando esté ordenado se utilizará para mostrar las palabras más utilizadas en orden descendente y al terminar se liberará la memoria
<br>
![Write del ranking de palabras](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/72359e17-6ed3-4528-ad04-924763568ced)
<br>
![Liberar memoria del diccionario temporal](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/43665e04-82e3-4f98-a666-378534416bda)
<br><br>
El tiempo total para implementar el diccionario y sus funcionalidades, incluyendo el _MergeSort_ han sido ocho horas. Es posible que se pueda mejorar la forma en la que mostramos las palabras más utilizadas en la aplicación, ya que duplicar un diccionario y ordenarlo parece demasiado simple pero no se nos ha ocurrido otra forma.
<br><br>

Las ubicaciones con el código relacionado al diccionario son:
- headers/WordDictionary.h: 12-20
- headers/WordDictionary.c: 10-131 (todo el archivo)
- main.c: 719-755, 897-914
<br><br>

Por último, queremos aclarar cómo hemos organizado nuestro repositorio de github. Tenemos la rama “main” que consiste en el proyecto principal, con interfaz gráfica y todas las funcionalidades, lo tenemos en la rama main para que sea más fácil distinguirlo y saber cual es nuestra entrega final, luego a parte tenemos dos ramas llamadas “Terminal” y “GUI” que son nuestros puntos de referencia. A la hora de desarrollar la aplicación estas dos ramas han sido nuestros dos “mains” y las hemos actualizado conforme íbamos desarrollando código limpio y revisado que pudiésemos subir a la versión buena. Como sus nombres indican, “Terminal” y “GUI” contienen las versiones más actualizadas y estables de la versión de terminal y la versión de interfaz gráfica respectivamente. “GUI” y “main” son exáctamente iguales.
<br><br>

### Objetivos deseables alcanzados

CSV Read/Write (solo en el implementado en la versión de terminal):<br>
Hemos diseñado un sistema de carga y guardado de datos de los usuarios, así como de sus listas de amigos y solicitudes de amistad recibidas y enviadas. La carga de datos sucede en cuanto se inicia el programa, con lo cual guarda toda la información de los usuarios previamente guardados. Si no hay, simplemente inicia el programa sin usuarios disponibles. Cuando se finaliza el programa, se guardan de nuevo los usuarios restantes. Las estructuras de los archivos CSV son estas:<br>
- Users: id,username,birthday,email,location,hobbies (1 to 5)
- Friends: Type (Friend/Friend request),Sender (ID),Receiver (ID).

<br><br>
![pasted image 30](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/17923abb-41cc-4a1c-ac8f-45816372b984)
<br>
![pasted image 31](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/1e29a498-ec8f-4dc5-8415-c3524b6d6a9b)
<br><br>
User Read:<br>
El código para leer información de un archivo se basa en un loop while en el que lee una línea por iteración:
<br>
![pasted image 32](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/c401accd-cd1d-48ca-bc58-b46356446c56)
<br><br>
El algoritmo se salta la primera línea, teniendo en cuenta que es el header. Cuando llega a una que contenga información, se activa un switch con todas las opciones:
<br>
![pasted image 33](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/3f3e6e33-43fe-4747-beb9-8e929efd5bcc)
<br>
![pasted image 34](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/8d79d923-38b7-4ca3-9f29-d62290ebcd94)
<br><br>
Como se puede observar, va guardando la información de cada columna en su respectiva variable, dentro de una variable de usuario previamente creada, entonces procede a leer el siguiente valor y suma 1 a la variable de columna para saber dónde lo guardará.
Una vez leídos los datos de la línea entera, tiene que llenar las variables que no se van a guardar ni cargar en este archivo y añadir el usuario a la lista de usuarios:
<br>
![pasted image 35](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/52129b3d-cb6a-4b78-ac10-583f164accb3)
<br><br>
User Write:<br>
El guardado de datos es mucho más sencillo. El algoritmo abre o crea el archivo y escribe la primera línea, el header. Entonces va iterando por la lista de usuarios y en cada línea guarda la información del usuario en cuestión (es posible que sea más eficiente abrir el archivo en modo append mientras no se eliminen usuarios).
<br>
![pasted image 36](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/addea3fc-f41e-47ad-b754-0dc9d4519ab5)
<br><br>
Friend Read:<br>
El guardado y la carga de información de las listas de amigos sigue la misma lógica pero contiene algunos cambios por la diferencia en la información guardada. Primero guarda los valores del tipo de amistad (“Friend Request” o “Friend”) y ambos IDs; esta parte del algoritmo es igual pero solo con 3 opciones en el switch. A partir de los IDs busca y encuentra los usuarios (una mejora aquí sería romper el loop una vez encontrados los usuarios).
<br>
![pasted image 37](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/20dafaeb-d63e-4185-8283-af2892633dd3)
<br><br>
Entonces, según el tipo de amistad, decide si añadir a una lista de amigos o de solicitudes de amistad. En el guardado de datos se repetirán las amistades en los 2 sentidos distintos (Friend,1,0 y Friend,0,1), por lo cual se necesita hacer una comprobación previa a traspasar la información a la lista de amigos del usuario:
<br>
![pasted image 38](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/2cdb652a-110c-4d31-9497-6f62ef0f1331)
<br><br>
Friend Write:
En el guardado de información de los amigos, va iterando por la lista de usuarios y guardando sus listas. Primero comprueba si el usuario tiene amigos o solicitudes de amistad (las listas no son NULL). En caso afirmativo, procede a iterar dentro de la lista para guardar el ID del usuario y el del amigo. Es necesario recalcar que solo se necesita recorrer la lista de solicitudes de amistad enviadas, dado que por cada solicitud de amistad enviada habrá una recibida. Las amistades sí que se verán duplicadas al guardar la información en el archivo:
<br>
![pasted image 39](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/50e9d04d-ead8-442c-95a5-56d5eacc30cd)
<br><br>
La implementación total de este sistema ha tardado 2 semanas, más 1 de revisión final. Está disponible en la rama de Terminal.
<br><br>

### Objetivos exploratorios adquiridos

El único objetivo libre que hemos implementado ha sido la interfaz gráfica, la cual como hemos hecho utilizando la SDK de Windows como hemos explicado anteriormente.
<br><br>
Programar una aplicación en interfaz gráfica es drásticamente diferente a programar una simple aplicación de interfaz de comandos y debemos explicar las diferencias en profundidad.
<br><br>
La función principal que se ejecuta al iniciar la aplicación cambia de _main_ a _WinMain_, que en nuestro caso es _wWinMain_ porque utilizamos wide characters en UTF-16. Esta función tiene unos argumentos especiales a diferencia de _main_, los cuales no son muy relevantes y están documentados en el código.
<br>
![pasted image 40](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/92639c22-9b23-416c-871a-f712ace591dd)
<br><br>
Pese a que C no presenta compatibilidad con programación orientada a objetos, en el _wWinMain_ lo primero que hacemos es crear y registrar la clase de la ventana principal en el sistema operativo para poder crear instancias de esta clase. Utilizamos estructuras y funciones predefinidas por la librería _windows.h_.
<br>
![pasted image 41](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/b759d7ac-6369-4cfe-8b8e-47a964ba3055)
<br><br>
El registro de clases de ventanas de la librería funciona como un diccionario, cuando utilicemos la clave “Main Window Class” creará una ventana con las propiedades que le hemos asignado a la clase, siendo la más importante el “procedure”, que en caso de la ventana principal es la función _WindProc_ de la que hablaremos más adelante. Posteriormente creamos una ventana de la clase registrada
<br>
![pasted image 42](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/4982d5fb-6591-49b1-9a06-0c548f01fa53)
<br><br>
El valor de retorno de crear una ventana es su _handle_, que actúa como una ID para utilizar en funciones de la librería cuando queramos modificar la ventana de alguna manera. Guardamos el handle de la ventana principal en una variable global y utilizamos _ShowWindow_ para que la ventana se muestre por pantalla.
<br>
![pasted image 43](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/18880f73-0ebf-4b97-b413-af3d315ea728)
<br><br>
Por último en _wWinMain_, tenemos que hablar de una diferencia fundamental entre las aplicaciones de consola y aquellas con interfaces gráficas. 
<br><br>
Asumiendo que no trabajamos con multithreading, en una aplicación C de consola el thread principal empezará llamando a main y de ahí seguirá el curso de la aplicación hasta que termine el _main_, momento en el cual la ejecución del thread finalizará y terminará también la ejecución de la aplicación. Esto significa que hagamos lo que hagamos, cuando el thread haya ejecutado todo y llegue al final, la aplicación terminará, lo que significa que si en el main de nuestra interfaz creamos una ventana, la mostramos y seguimos hasta el return, al iniciar la aplicación mostrará la ventana por una fracción de segundo y luego terminará la aplicación. 
<br><br>
En una aplicación, al igual que es importante recordar cerrar archivos después de utilizarlos, también tenemos que mantener el thread ocupado en un bucle que esté constantemente alerta de interacciones del usuario con la interfaz. En una aplicación de consola el usuario tiene input solo cuando y donde el programa lo pida mientras que con una interfaz no hay un orden definido, el usuario tiene que poder tocar cualquier botón o elemento y el programa ha de reaccionar a tiempo real.
<br><br>
En el caso de WinAPI el thread escucha los mensajes que manda el sistema operativo a la ventana y los procesa en el “Procedure”.
<br>
![pasted image 44](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/9c1c3579-5089-4862-996a-233c0084604d)
<br><br>
A continuación tenemos el Procedure, esta es la función a la que se envían los mensajes del sistema operativo o las acciones del usuario. Cuando se ejecuta la función porque ha recibido un mensaje recibirá siempre como parámetros el handle de la ventana que lo ha recibido, el mensaje que se ha enviado y dos parámetros que dependen del mensaje enviado.
<br>
![pasted image 45](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/b85a831b-1bb8-4239-887a-d41f665e53c6)
<br><br>
El Procedure consiste únicamente en un switch que actúa dependiendo del mensaje.
<br>
![pasted image 46](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/a3f21158-9ec6-4fba-b3f6-f467955807cf)
<br><br>
Dentro del switch tenemos instrucciones para cuando la ventana se cree (_WM_CREATE_), se cierre (_WM_CLOSE_), se destruya (_WM_DESTROY_), se redimensiona (_WM_SIZE_), etc. La librería incluye una opción para hacer las acciones predefinidas en cada caso para no tener que especificar todos los mensajes posibles. 
<br>
![pasted image 47](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/34eb57dc-1b9d-4fc8-8100-4b5a79f875b0)
<br><br>
Es muy importante agregar elementos con los que el usuario pueda interactuar para que la aplicación funcione correctamente y esto lo hemos hecho principalmente con botones y cajas de texto, aunque también tenemos un menú para las acciones que no son específicas del usuario. Todas las partes de la interfaz gráfica se crean como si fueran ventanas, así que utilizamos el comando _CreateWindowExW_ para crear ventanas, botones, textos estáticos, cajas de texto, etc.
<br>
![pasted image 48](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/1ecc74f5-c194-4a2f-8d15-1b22976c188d)
<br><br>
Todos los botones y menús (en rojo) han sido creados con un parámetro que indica que número envían junto al mensaje cuando son pulsados.
<br>
![pasted image 49](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/2cccd68a-786a-407d-9297-14dc041de3f1)
<br>
![pasted image 50](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/df46a69e-dc65-4936-bec7-8b2f324cd001)
<br><br>
Al hacer clic en un botón se envía un mensaje de tipo _WM_COMMAND_ y en el la parte baja del parámetro _WPARAM_, la cual podemos sacar con una macro de la librería, contiene el número previamente mencionado
<br>
![pasted image 51](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/14048629-db3c-43fc-8f41-bb8a6e0ca3e1)
<br>
![pasted image 52](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/e4d18c64-a781-4e6c-864b-13b0588f25a9)
<br><br>
En resumen, así es cómo gestionamos que la aplicación lea el input del usuario. Ahora queremos explicar cómo hemos gestionado lo que se conoce como _responsiveness_, que consiste en adaptar una aplicación para que los elementos de la interfaz se adapten al tamaño de la ventana.
<br><br>
A diferencia de HTML y CSS, el tamaño de las ventanas en WinAPI solo puede ser definido con números enteros indicando el número de píxeles. Si dejamos un valor fijo al hacer más grande o pequeña la ventana el tamaño se quedará igual y esto haría que tener la aplicación a pantalla completa sea una experiencia completamente diferente de un monitor a otro, incluso llegando a “romper” la aplicación si, por ejemplo, la textbox del chat no llega a poder verse porque la ventana es muy pequeña.
<br><br>
Para solventar esto hemos definido una función _LoadWindow_ que se llama cuando la pantalla se redimensiona (mensaje _WM_SIZE_) y guardamos las dimensiones actuales de la ventana de forma global para poder acceder desde cualquier parte del código.
<br>
![pasted image 53](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/1ec03bba-a39a-40fe-8815-82da0746345e)
<br><br>
Dentro de _LoadWindow_ dividimos el tamaño horizontal de la ventana en doce en lo que llamamos “width_unit”. Nos decidimos por doce debido a que es divisible por dos, tres, cuatro y seis, es bastante cómodo para posicionar los diferentes elementos. También es el número en el que se dividen las tablas en Bootstrap, una librería de HTML, CSS y JS para diseño web.
<br>
![pasted image 54](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/7b3544c0-9d32-464b-b83f-e7a38b2defb7)
<br>
![pasted image 55](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/cabbc904-4b17-443d-9994-4a7ae2600b59)
<br>
![pasted image 56](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/18981d22-5c53-4e22-a190-c938bebc6465)
<br><br>
La proporción horizontal de la ventana va a ser por lo tanto siempre la misma sin importar el tamaño. No hemos creado una “length_unit” porque no hay muchas longitudes dinámicas y la mayoría son el 100% de la largaria de la pantalla, aunque en la parte central si dividimos la longitud entre ocho y asignamos una unidad a la parte inferior de caja de texto + botón y siete unidades en la parte superior, que contiene la Timeline del usuario. 
<br><br>
Es en esta función donde hacemos uso del Stack de handles para borrar todo lo que hay por pantalla y volverlo a crear nuevamente. Primero vaciamos el stack destruyendo todas las ventanas y luego vamos agregando los handles de las ventanas conforme las vamos creando.
<br>
![pasted image 57](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/7eb9e0c1-8fd0-4761-aaf0-80b260bacd02)
<br>
![pasted image 58](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/542f59b7-b01a-47b0-8f79-1c74da3047bb)
<br><br>
También hemos definido un tamaño mínimo para que el usuario no pueda romper la ventana haciéndola minúscula
<br>
![pasted image 59](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/4f839ee1-9367-4c4b-be4a-a3fa770233ea)
<br><br>
Debido a todos los cálculos que requiere hacer una ventana responsive hemos hecho únicamente esto en la ventana principal y las ventanas secundarias tienen las posiciones de sus subelementos fijas, y hablando de ventanas secundarias, es un buen momento para hablar de la creación de usuario en interfaz gráfica para poner un ejemplo detallado de cómo gestionamos la interacción entre el usuario y la aplicación.
<br><br>
Tenemos una segunda clase de ventana registrada con un segundo procedure para todas las ventanas secundarias que creamos por encima de la principal. Cuando el usuario hace clic en el submenú para crear un usuario se crea un formulario.
<br>
![pasted image 60](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/3783c59d-97a9-43fa-bbe5-91237fa44873)
<br>
![pasted image 61](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/f7fde35d-d231-40d4-b998-e5d2d3f1f14a)
<br><br>
Al rellenar los campos correctamente y pulsar el botón, un “popup” aparece notificando que el usuario se ha creado, o que ha habido un problema en uno de los campos y no se ha podido crear. Si el usuario se crea correctamente se cierra la ventana y se añade a la lista global de usuarios.
<br>
![pasted image 62](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/b0fc3487-20f1-4d9f-bf09-bd5913e7c025)
<br><br>
Habiendo creado ya usuarios, tenemos la opción de elegir un usuario como el que actuar mediante Administración -> Operar como usuario, esto crea una ventana secundaria donde se puede elegir un usuario de los ya creados para seleccionarlo como usuario activo.
<br>
![pasted image 63](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/cba31e38-5d37-411e-a7fb-c7ddd5961c2b)
<br><br>
Las solicitudes de amistad funcionan de la misma manera, para enviar, aceptar o cancelar solicitudes de amistad se crea una ventana con un botón para cada usuario. Este botón tiene como texto el nombre del usuario seguido de su id y al pulsarlos podemos coger su texto y buscar al usuario a partir de la id. Recordamos que en caso de pulsar un botón LPARAM contiene el handle del botón.
<br>
![pasted image 64](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/acc652c6-95ff-433f-879d-4cd2e15650f9)
<br><br>
Para terminar con la interfaz, queremos hablar de cómo presentamos las palabras más utilizadas en la plataforma. En todos los posts se cuenta cada palabra separada por un espacio, salto de línea o tabulación. Al pulsar el botón de “Ver detalles” podemos ver los detalles del usuario junto al ranking de palabras más utilizadas
<br>
![pasted image 65](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/046cb037-51f3-482f-b1a8-471a7c010c53)
<br>
![pasted image 66](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/7123a8c3-a8ec-4bc6-8b68-d45696449b81)
<br><br>
Este ranking se obtiene clonando el diccionario, ordenándolo y escribiendo en la caja de texto.
<br>
![pasted image 67](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/8268db4e-7ff4-46e1-8186-dd29eba67246)
<br><br>
La interfaz gráfica es lo que más nos ha costado en todo el proyecto y su tiempo estimado sería más del 60% del tiempo total dedicado. El código se encuentra en _main.c_, todo el archivo.
<br><br>

## SOLUCIÓN

### Arquitectura del sistema

Adjuntamos un diagrama de la relación entre las diferentes estructuras.
<br>
![Diagrama_estructura_light](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/703e33b7-4c3a-4e6e-912e-a327676a93da)
<br><br>
En main tenemos todo lo relacionado con la interfaz. Las funciones están contenidas en sus bloques así que lo único importante es la relación de sus estructuras.

### Gestión de errores

Como hemos hecho una interfaz gráfica, hemos tenido muchos más problemas de los que tenemos normalmente. Hemos podido ahorrarnos muchos problemas siguiendo una estructura y un orden desde el principio, los errores de implementación simplemente los hemos resuelto utilizando una mezcla de prints estratégicos en partes clave del código y siguiendo la aplicación con el modo debug.

### Diseño del modelo de datos

Como hemos mencionado previamente, tener una interfaz gráfica significa que en lugar de ser el programa el que dicta que puede hacer el usuario en todo momento, el usuario es el que ha de ser capaz de interactuar con la aplicación, es por eso que hemos preparado un diagrama de flujo en el que no hay un punto central como lo sería el menú de una aplicación por consola, sino que el usuario es libre de escoger la opción que quiera entre todos los botones y menús disponibles.
<br>
![Diagrama_flujo_completo_2](https://github.com/jbelloncordobes/EDA2_Lab/assets/65515467/6f0c8a23-f568-46c9-801e-df8ee8b1aaad)
<br><br>
### Descripción y procesamiento del conjunto de datos 

Para gestionar la aplicación, los datos se guardan en variables globales. Por ejemplo tenemos un diccionario global para todos los usuarios y una lista global con todos los usuarios, además del handle de la ventana principal y un stack con todos los handles para actualizar fácilmente la interfaz. Nuestras estructuras consisten principalmente en números enteros y wide chars, y son:
- Usuarios
    - Arrays de usuarios
    - Colas de Posts
        - Posts
- Diccionarios
- Stacks de handles

## REFERENCIAS

[Documentación de microsoft para WinAPI](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list), en concreto la sección de [crear tu primera app](https://learn.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows).
<br><br>
Tutorial de YouTube [Windows GUI Programming with C/C++ ( Win32 API ) por The Pentamollis Project](https://www.youtube.com/playlist?list=PLWzp0Bbyy_3i750dsUj7yq4JrPOIUR_NK)
<br><br>
Posts variados de [StackOverflow](https://stackoverflow.com/) para problemas concretos de WinAPI o wide chars.
<br><br>
[ChatGPT](https://chat.openai.com/) para obtener más documentación y resolver problemas puntuales de WinAPI
<br><br>
Algoritmo MergeSort sacado de [GeeksforGeeks](https://www.geeksforgeeks.org/merge-sort/)
<br><br>
Vídeo de YouTube [Read CSV File Data Into An Array Of Structs por Portfolio Courses](https://www.youtube.com/watch?v=rbVt5v8NNe8) y [GeeksforGeeks](https://www.geeksforgeeks.org/relational-database-from-csv-files-in-c/) para lectura y escritura de CSVs en C<br>
