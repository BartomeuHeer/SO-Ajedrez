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
    public partial class FormInicio : Form
    {
        Socket server;
        Thread atender;
        //string invitados;
        public FormInicio()
        {
            InitializeComponent();
            //CheckForIllegalCrossThreadCalls = false; //Necesario para que los elementos de los formularios puedan ser
            //accedidos desde threads diferentes a los que los crearon
        }
        private void show_inicio()
        {
            label1.Visible = true;
            label3.Visible = true;
            tbUsernameLI.Visible = true;
            label4.Visible = true;
            tbPassLI.Visible = true;
            btLogin.Visible = true;
            label2.Visible = true;
            tbName.Visible = true;
            tbAge.Visible = true;
            tbLN1.Visible = true;
            tbUserNameR.Visible = true;
            tbLN2.Visible = true;
            tbPassR.Visible = true;
            btRegister.Visible = true;
            label5.Visible = true;
            label6.Visible = true;
            label7.Visible = true;
            label8.Visible = true;
            label9.Visible = true;
            label10.Visible = true;
            btndesc.Visible = true;
        }
        private void show_consultas()
        {
            rb1.Invoke(new MethodInvoker(delegate { rb1.Visible = true; }));
            rb2.Invoke(new MethodInvoker(delegate { rb2.Visible = true; }));
            rb3.Invoke(new MethodInvoker(delegate { rb3.Visible = true; }));
            btBuscar.Invoke(new MethodInvoker(delegate { btBuscar.Visible = true; }));
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
                    case 1:  //respuesta del log in

                        if (mensaje == "0")
                        {
                            MessageBox.Show("Inicio de sesion correcto");
                            show_consultas();
                        }
                        else if (mensaje == "-1")
                            MessageBox.Show("Inicio de sesion incorrecto");
                        else
                            MessageBox.Show("Hay demasiados jugadores conectados");
                        break;
                    case 2:      //respuesta del registro

                        if (mensaje == "0")
                        {
                            MessageBox.Show("Registro correcto");
                        }
                        else
                        {
                            MessageBox.Show("Registro incorrecto");
                        }
                        break;
                    case 3:       //Respuesta primera consulta
                        int total;
                        string[] info;
                        if (mensaje == "-2")
                            MessageBox.Show("No se han encontrado datos para esta consulta");
                        else if (mensaje == "-1")
                            MessageBox.Show("Error al hacer la consulta");
                        else
                        {
                            total = Int32.Parse(mensaje.Split('/')[0]);
                            info = mensaje.Split('/');
                            string[] nombres = new string[50];
                            string[] apellido1 = new string[50];
                            string[] apellido2 = new string[50];
                            for (int i = 1; i <= total; i++)
                            {
                                nombres[i - 1] = info[i].Split(',')[0];
                                apellido1[i - 1] = info[i].Split(',')[1];
                                apellido2[i - 1] = info[i].Split(',')[2];
                            }
                            FormPrimeraBus sel = new FormPrimeraBus(nombres, apellido1, apellido2, total);
                            sel.ShowDialog();
                        }
                        break;
                    case 4:     //Respuesta segunda consulta
                       
                        if (mensaje == "-2")
                            MessageBox.Show("No se han encontrado datos para esta consulta");
                        else
                        {
                            total = Int32.Parse(mensaje.Split('/')[0]);
                            info = mensaje.Split('/');
                            string[] nom = new string[50];
                            string[] cognom1 = new string[50];
                            string[] cognom2 = new string[50];
                            for (int i = 1; i <= total; i++)
                            {
                                int v = i - 1;
                                nom[v] = info[i].Split(',')[0];
                                cognom1[i - 1] = info[i].Split(',')[1];
                                cognom2[i - 1] = info[i].Split(',')[2];
                            }
                            FormPrimeraBus sel = new FormPrimeraBus(nom, cognom1, cognom2, total);
                        }
                        break;
                    case 5: //Respuesta tercera consulta

                        break;
                    case 6:
                        dataGridConect.Invoke(new MethodInvoker(delegate { dataGridConect.Rows.Clear(); })); 
                        dataGridConect.Invoke(new MethodInvoker(delegate { dataGridConect.Refresh(); }));
                        total = Int32.Parse(mensaje.Split('/')[0]);
                        info = mensaje.Split('/');
                        for (int i = 1; i <= total; i++)
                        {
                            dataGridConect.Invoke(new MethodInvoker(delegate { dataGridConect.Rows.Add(info[i]); }));
                        }
                        break;
                    case 7:
                        int partida = Int32.Parse(mensaje.Split('/')[0]);
                        string m = mensaje.Split('/')[1];
                        if (m == "0")
                            MessageBox.Show("Ha habido un error");
                        else
                        {
                            DialogResult dialogResult = MessageBox.Show("¿Aceptar invitacion de " + m + "?", "Invitacion a partida", MessageBoxButtons.OKCancel);
                            if(dialogResult == DialogResult.OK)
                            {
                                m = "7/0-" + partida +"|"+ m;
                                byte[] msg = Encoding.ASCII.GetBytes(m);
                                server.Send(msg);
                            }
                            else
                            {
                                m = "7/1-" + partida + "|" + m;
                                byte[] msg = Encoding.ASCII.GetBytes(m);
                                server.Send(msg);
                            }
                        }
                        break;
                    case 8:
                        int numPart;
                        int numJug;
                        Partida part;
                        Jugador[] jug = new Jugador[4];
                        string[] inf = mensaje.Split('/');
                        numPart = Int32.Parse(inf[1]);
                        if (Int32.Parse(inf[0]) == 0)
                        {
                            numJug = Int32.Parse(inf[2]);
                            for (int i = 3; i < numJug + 3; i++)
                            {
                                jug[i - 3] = new Jugador();
                                jug[i - 3].setNombre(inf[i]);
                            }
                            part = new Partida(jug,numJug,numPart);
                            FormPartida fp = new FormPartida(server, atender, part);
                            fp.ShowDialog();
                        }
                        else
                        {
                            MessageBox.Show("Un participante ha rechazado la invitacion.");
                        }

                        break;
                }
            }
        }
        private void btnConect_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9050);


            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");
                show_inicio();
            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                this.BackColor = Color.Red;
                MessageBox.Show("No he podido conectar con el servidor");
            }
            //pongo en marcha el thread que atenderá los mensajes del servidor
            ThreadStart ts = delegate { AtenderServidor(); };
            atender = new Thread(ts);
            atender.Start();
        }

        private void btLogin_Click(object sender, EventArgs e)
        {
            string mensaje;
            if (!string.IsNullOrEmpty(tbUsernameLI.Text) || !string.IsNullOrEmpty(tbPassLI.Text)) {
                mensaje = "1/" + tbUsernameLI.Text + "/" + tbPassLI.Text;
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
                MessageBox.Show("Faltan cmapos por rellenar.");
        }

        private void btRegister_Click(object sender, EventArgs e)
        {
            string mensaje;
            if (!string.IsNullOrEmpty(tbName.Text) || !string.IsNullOrEmpty(tbLN1.Text) || !string.IsNullOrEmpty(tbLN2.Text) || !string.IsNullOrEmpty(tbUserNameR.Text) || !string.IsNullOrEmpty(tbPassR.Text) || !string.IsNullOrEmpty(tbAge.Text))
            {
                mensaje = "2/" + tbName.Text + "/" + tbLN1.Text + "/" + tbLN2.Text + "/" + tbUserNameR.Text + "/" + tbPassR.Text + "/" + tbAge.Text;
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
                MessageBox.Show("Faltan cmapos por rellenar.");
        }

        
        private void btBuscar_Click(object sender, EventArgs e)
        {
            if (rb1.Checked)
            {
                string mensaje = "3/";
                // Enviamos al servidor la peticion
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

            }
            else if (rb2.Checked)
            {
                string mensaje = "4/";
                // Enviamos al servidor el nombre tecleado
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }

        private void btndesc_Click(object sender, EventArgs e)
        {
            //Mensaje de desconexión
            string mensaje = "0/";

            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Nos desconectamos
            atender.Abort();
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
            MessageBox.Show("Desconexion del servidor completada");
        }

        private void btnInv_Click(object sender, EventArgs e)
        {
            string mensaje = "";


            if (dataGridConect.SelectedRows.Count < 1 || dataGridConect.SelectedRows.Count > 3)
            {
                MessageBox.Show("Selecciona a 1,2 o 3 jugadores por favor.");
            }
            else
            {
                int i = 0;
                foreach(DataGridViewRow row in dataGridConect.SelectedRows)
                {
                    mensaje = mensaje + row.Cells[0].Value.ToString() + ",";
                    i++;
                }
                mensaje = "6/" + i + "/" + mensaje;    
                //mensaje.Remove(mensaje.Length - 1);
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }
    }
}
