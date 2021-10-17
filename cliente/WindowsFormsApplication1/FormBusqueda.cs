using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication1
{
    public partial class FormBusqueda : Form
    {
        Socket server;
        public FormBusqueda()
        {
            InitializeComponent();
        }
        public void setSocket(Socket s)
        {
            this.server = s;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //Mensaje de desconexión
            string mensaje = "0/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Nos desconectamos
            
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            if (rb1.Checked)
            {
                string mensaje = "3/";
                // Enviamos al servidor el nombre tecleado
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                int total = Int32.Parse(mensaje.Split('/')[0]);
                string[] info = mensaje.Split('/');
                string[] nombres = new string[50];
                string[] apellido1 = new string[50];
                string[] apellido2 = new string[50];
                for (int i = 1; i <= total; i++)
                {
                    nombres[i - 1] = info[i].Split(',')[0];
                    apellido1[i - 1] = info[i].Split(',')[1];
                    apellido2[i - 1] = info[i].Split(',')[2];
                }
                FormPrimeraBus sel = new FormPrimeraBus(nombres,apellido1,apellido2,total);

            }
        }
    }
}
