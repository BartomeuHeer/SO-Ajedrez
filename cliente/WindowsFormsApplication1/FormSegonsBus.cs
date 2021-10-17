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
    public partial class FormSegonsBus : Form
    {
        string[] nom;
        string[] cognom1;
        string[] cognom2;
        int total;
        public FormSegonsBus(string[] n, string[] c1, string[] c2, int t)
        {
            InitializeComponent();

            InitializeComponent();
            nom = n;
            cognom1 = c1;
            cognom2 = c2;
            total = t;

            for (int i = 0; i < total; i++)
            {
                dataGridView2.Rows[i].Cells[0].Value = nom[0];
                dataGridView2.Rows[i].Cells[1].Value = cognom1[0];
                dataGridView2.Rows[i].Cells[2].Value = cognom2[0];

            }
        }

        private void dataGridView2_CellContentClick(object sender, DataGridViewCellEventArgs e)
        { }
    }
}
