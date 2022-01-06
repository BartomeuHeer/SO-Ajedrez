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
        List<PictureBox> obstaculos = new List<PictureBox>();
        const int MIN_POS_FIELD = 596;
        const int MAX_POS_FIELD = 12;
        string textoChat = "";
        Socket server;
        Partida partida;
        int min,sec,ms, index;
        System.Timers.Timer tiempoInicio,tiempoContador;
        bool space_pressed;
        string[] cuentaAtras = { "3", "2", "1", "GO!" };
        public FormPartida(Socket s, Partida p, int nf)
        {
            InitializeComponent();
            this.server = s;
            this.partida = p;
            for (int i = 0; i < partida.getNumJugadores(); i++)
            {
                dataGridView1.Rows.Add(partida.getJugadores()[i].getNombre());
            }
            string mensaje = "8/" + partida.getNum() + "/" + nf;
            lbInfo.Parent = pictureBox1;
            lbInfo.BackColor = Color.Transparent;
            pictureBox2.Parent = pictureBox1;
            pictureBox2.BackColor = Color.Transparent;
            lblTiempo.Parent = pictureBox1;
            lblTiempo.BackColor = Color.Transparent;
            //pictureBox2.Location = new Point(532, 338);
        }
        
        private void btnEnviar_Click(object sender, EventArgs e)
        {
            string mensaje;
            if (!string.IsNullOrEmpty(tbDecir.Text))
            {
                mensaje = "9/" + partida.getNum() + "/" + tbDecir.Text;
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                
            }
        }
        public void chat(string mensaje)
        {
            textoChat = textoChat +  mensaje + "\r\n";
            tbChat.Invoke(new MethodInvoker(delegate { tbChat.Clear(); }));
            tbChat.Invoke(new MethodInvoker(delegate { tbChat.Text = textoChat; }));
        }
        public delegate void KeyPressedEventHandler(object source, KeyPressEventArgs arg);
        private void FormPartida_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == ' ')
                space_pressed = true;
        }

        private void btnListo_Click(object sender, EventArgs e)
        {
            string mensaje;
            mensaje = "10/" + partida.getNum();
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }
        public void empezarPartida()
        {
            index = 0;
            //TimerCallback timerCallback = new TimerCallback(contadorInicial);
            tiempoInicio = new System.Timers.Timer(1000);
            tiempoInicio.Enabled = true;
            tiempoInicio.Elapsed += TiempoInicio_Elapsed;
            space_pressed = false;
        }

        private void TiempoInicio_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (index == 4)
                lbInfo.Invoke(new MethodInvoker(delegate { lbInfo.Text = "Pulsa espacio para saltar los objetos."; }));
            if (index == 6)
            {
                lbInfo.Invoke(new MethodInvoker(delegate { lbInfo.Text = ""; }));
                tiempoInicio.Dispose();
                return;
            }
            if (index < 4)
            {
                lbInfo.Invoke(new MethodInvoker(delegate { lbInfo.Text = cuentaAtras[index]; }));
            }
            if (index == 3)
            {
                //TimerCallback timerCallback = new TimerCallback(contadorJuego);
                tiempoContador = new System.Timers.Timer(1);
                tiempoContador.Enabled = true;
                tiempoContador.Elapsed += TiempoContador_Elapsed;
                min = 0;
                sec = 0;
                ms = 0;
            }
            index++;
        }

        private void TiempoContador_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            ms++;
            if (ms % 2 == 0 && pictureBox2.Location.Y <= MIN_POS_FIELD && !space_pressed)
            {
                pictureBox2.Invoke(new MethodInvoker(delegate { pictureBox2.Location = new Point(pictureBox2.Location.X, pictureBox2.Location.Y + 2); }));
            }
            else if (ms % 2 == 0 && space_pressed)
            {
                int y = pictureBox2.Location.Y - 6;
                if(y < MAX_POS_FIELD)
                    pictureBox2.Invoke(new MethodInvoker(delegate { pictureBox2.Location = new Point(pictureBox2.Location.X, MAX_POS_FIELD); }));
                else
                    pictureBox2.Invoke(new MethodInvoker(delegate { pictureBox2.Location = new Point(pictureBox2.Location.X, y); }));
                space_pressed = false;
            }
            //cargarObstaculos();
            contadorJuego();
           
        }
        private void contadorJuego()
        {
            if (ms == 100)
            {
                ms = 0;
                sec++;
            }
            if (sec == 60)
            {
                sec = 0;
                min++;
            }
            TimeSpan ts = new TimeSpan(0, 0, min, sec, ms);
            string tiempoMin = ts.Minutes.ToString().Length < 2 ? "0" + ts.Minutes.ToString() : ts.Minutes.ToString();
            string tiempoSec = ts.Seconds.ToString().Length < 2 ? "0" + ts.Seconds.ToString() : ts.Seconds.ToString();
            string tiempoMs = ts.Milliseconds.ToString().Length < 2 ? "0" + ts.Milliseconds.ToString() : ts.Milliseconds.ToString();
            lblTiempo.Invoke(new MethodInvoker(delegate { lblTiempo.Text = tiempoMin + ":" + tiempoSec + ":" + tiempoMs; }));
        }

        private void cargarObstaculos()
        {
            if (sec % 2 == 0)
            {
                Random r = new Random();
                switch (r.Next(1, 5))
                {
                    case 1:
                        PictureBox pbObs1 = new PictureBox();
                        pbObs1.Image = Image.FromFile(@"D:\Documentos\Uni\Tercer curs\SO\SO-Ajedrez\cliente\WindowsFormsApplication1\Img\rock.png");
                        pbObs1.Location = new Point(1530, r.Next(13, 580));
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                }
            }
        }
    }
}
