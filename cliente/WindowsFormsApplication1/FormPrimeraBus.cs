using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class FormPrimeraBus : Form
    {
        string[] nombres;
        string[] apellido1;
        string[] apellido2;
        int total;
        public FormPrimeraBus(string[] n, string[] a1, string[] a2,int t)
        {
            InitializeComponent();
            this.nombres = n;
            this.apellido1 = a1;
            this.apellido2 = a2;
            this.total = t;

            for (int i = 0; i < total; i++)
            {

                dataGridView1.Rows[i].Cells[0].Value = nombres[0];
                dataGridView1.Rows[i].Cells[1].Value = apellido1[0];
                dataGridView1.Rows[i].Cells[2].Value = apellido2[0];
            }
        }

    }
}
