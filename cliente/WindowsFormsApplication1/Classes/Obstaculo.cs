using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1.Classes
{
    class Obstaculo
    {
        public Obstaculo(PictureBox pb)
        {
            this.tocado = false;
            this.img = pb;
        }
        private PictureBox img;
        private bool tocado;

        public PictureBox getImg()
        {
            return this.img;
        }
        public void setImg(PictureBox image)
        {
            this.img = image;
        }
        public bool getTocado()
        {
            return this.tocado;
        }
        public void setTocado(bool t)
        {
            this.tocado = t;
        }
    }
}
