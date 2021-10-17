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
    public partial class FormInicio : Form
    {
        Socket server;
        public FormInicio()
        {
            InitializeComponent();
        }

        public void setSocket(Socket s)
        {
            this.server = s;
        }
        private void btLogin_Click(object sender, EventArgs e)
        {
            string mensaje = "1/" + tbUsernameLI.Text + "/" + tbPassLI.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            if(mensaje == "0")
            {
                MessageBox.Show("Inicio de sesion correcto");
                FormBusqueda bus = new FormBusqueda();
                bus.setSocket(server);
                this.Close();
            }
            else
                MessageBox.Show("Inicio de sesion incorrecto");
        }

        private void btRegister_Click(object sender, EventArgs e)
        {
            /*string mensaje = "2/" + tbName.Text + "/" + tbLN1.Text + "/" + tbLN2.Text + "/" + tbAge.Text + "/" + tbUserNameR.Text + "/" + tbPassR.Text;
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            if (mensaje == "0")
            {
                MessageBox.Show("Registro correcto");
                FormBusqueda bus = new FormBusqueda();
                bus.setSocket(server);
                this.Close();
            }
            else
            {
                MessageBox.Show("Registro incorrecto");
                return;
            }
              */  
            
        }
    }
}
