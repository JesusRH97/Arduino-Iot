/*
 * Modificación del sketch de ejemplo WebServer incluido en el IDE de Arduino.
 * Se ha inculido un formulario que permite controlar remotamente un LED
 */

#include <SPI.h>
#include <Ethernet.h>

//Configurar dirección MAC e IP
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 5);

//VARIABLES
String cadenaHTTP;  //almacena la cadena HTTP de la peticion del cliente
boolean estado;     //estado de la salida
int pinSalida = 3;  //pin que vamos a controlar

// Iniciar servidor web en puerto 80
EthernetServer server(80);

void setup()
{
  //iniciar variables
  estado=false; //estado inicial del control: OFF
  cadenaHTTP=""; //cadena vacia
  pinMode(pinSalida,OUTPUT);
  
  //Puerto serie para control y depuración
  Serial.begin(9600);
 
  //Iniciar módulo Ethernet y servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("servidor web en ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  //escuchar peticiones de clientes
  EthernetClient client = server.available();
  //si se conecta un cliente proceder a enviar html y procesar formulario
  if (client)
  {
    Serial.println("Client conectado");
    
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        cadenaHTTP += c;  //añadir caracter a cadena HTTP
        
        //cuando se detecte la doble línea es que ha terminado la solicitudo GET
        if (c == '\n' && currentLineIsBlank)        {

           procesarPeticion();  //procesar la cadena de la peticion para actualizar las variables
           digitalWrite(pinSalida,estado);
           
          //enviar HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                              
          dibujarFormulario(client); //dibujar formulario
          
          client.println("</html>");
          cadenaHTTP="";              //borrar cadena para empezar de nuevo
          break;
        }

        //detección de la secuencia /n/r para comprobar el fin de la petición
        if (c == '\n')      currentLineIsBlank = true;
        else if (c != '\r') currentLineIsBlank = false;
      }
    }
    //dar tiempo al navegador para recibir los datos
    delay(1);
    //finalizar conexión
    client.stop();
    Serial.println("cliente desconectado");
  }
}

/*************  funcion: procesarPeticion **************/
 //descripcion: lee el estado de la peticion HTTP y busca en coincidencias en el valor enviado .
 //en caso de haber cambios actualiza las variables
 void procesarPeticion()
{    
    //mirar si se ha cambiado el estado
    if(cadenaHTTP.indexOf("GET /?estado=ON")>-1)
    {
      estado=true;
      Serial.println("Recibida peticion ON");
    }
    else if(cadenaHTTP.indexOf("GET /?estado=OFF")>-1)
    {
       estado=false;  
       Serial.println("Recibida peticion OFF");
    }
}

/*************  funcion: dibujarFormulario **************/
 //descripcion: genera el código HTML para el formulario en función del estado de la variable.
 void dibujarFormulario(EthernetClient client)
{      
     //dibujar formulario con botones de radio
     client.println("<form><fieldset style='width:300px;'><legend>Control salida:</legend><br>Estado<br>");
     
     //codigo de botones. se marca como checked el que corresponda según estado    
     client.print("<input type='radio' name='estado' value='ON' onclick='submit()'");
     if (estado) client.print(" checked='checked'");
     client.println(">ON<br>");
     
     client.print("<input type='radio' name='estado' value='OFF' onclick='submit()'");
     if (!estado) client.print(" checked='checked'");
     client.println(">OFF<br>");
             
     client.println("</fieldset></form>");    
}

