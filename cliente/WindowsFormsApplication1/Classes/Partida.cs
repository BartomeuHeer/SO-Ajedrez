using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsApplication1.Classes
{
    public class Partida
    {
        public Partida(Jugador[] j,int nj,int n)
        {
            this.numJugadores = nj;
            this.jugadores = j;
            this.num = n;
        }
        private int num;
        private Jugador[] jugadores;
        private int numJugadores;
        public int getNum()
        {
            return this.num;
        }
        public void setNum(int n)
        {
            this.num = n;
        }
        public Jugador[] getJugadores()
        {
            return this.jugadores;
        }
        public void setJugadores(Jugador[] j)
        {
            this.jugadores = j;
        }
        public int getNumJugadores()
        {
            return this.numJugadores;
        }
        public void setNumJugadores(int nj)
        {
            this.numJugadores = nj;
        }
        

    }
}
