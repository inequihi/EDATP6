Ejemplo que muestra:
Uso no bloqueante/asincronico de curl con curl_multi_perform.
Solicitud de twits a la API de twitter, mas info en: https://developer.twitter.com/en/docs/tweets/search/overview
Twitter les devuelve un archivo de formato JSON, si quieren saber mas sobre esos archivos: https://www.json.org/json-en.html
Igual no necesitan saber tanto, si abren el archivo "json example.JSON" en visual van a ver como es la estructura, es bastante
	legible. Usamos la libreria de JSON "json.hpp", en el main se muestra como leer lo que manda twitter. Si quieren ver
	completo lo que manda Twitter, pueden imprimir la variable readString del ejemplo o guardarla en un archivo. 
