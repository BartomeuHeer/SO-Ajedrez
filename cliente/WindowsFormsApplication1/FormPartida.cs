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
using System.Threading;
using WindowsFormsApplication1.Classes;

namespace WindowsFormsApplication1
{

    public partial class FormPartida : Form
    {
        Socket server;
        Thread atender;
        Partida partida;
        public FormPartida(Socket s, Thread t, Partida p)
        {
            InitializeComponent();
            this.server = s;
            this.atender = t;
            this.partida = p;
            //pictureBox1.Image = Image.FromFile(@"Img\fondo.png");
            for (int i = 0; i < partida.getNumJugadores(); i++)
            {
                dataGridView1.Rows.Add(partida.getJugadores()[i].getNombre());
            }
            

        }
        private void AtenderServidor()
        {
            while (true)
            {
                //Recibimos mensaje del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('|');
                int codigo = Convert.ToInt32(trozos[0]);
                string mensaje = trozos[1].Split('\0')[0];

                switch (codigo)
                {
                    case 9:
                        tbChat.Invoke(new MethodInvoker(delegate { tbChat.Text = mensaje + "\n"; }));
                        break;
                }
            }
        }
        private void btnEnviar_Click(object sender, EventArgs e)
        {
            string mensaje;
            if (!string.IsNullOrEmpty(tbDecir.Text))
            {
                mensaje = "8/" + partida.getNum() + "/" + tbDecir.Text;
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }
    }
}
