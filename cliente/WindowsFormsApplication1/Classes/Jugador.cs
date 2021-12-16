using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace WindowsFormsApplication1.Classes
{
    public class Jugador
    {
        private string nombre;

        public string getNombre()
        {
            return this.nombre;
        }
        public void setNombre(string n)
        {
            this.nombre = n;
        }
        private Stopwatch tiempo;
        public Stopwatch getTiempo()
        {
            return this.tiempo;
        }
        public void setTiempo(Stopwatch sw)
        {
            this.tiempo = sw;
        }
    }
}
