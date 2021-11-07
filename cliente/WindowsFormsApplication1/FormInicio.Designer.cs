
namespace WindowsFormsApplication1
{
    partial class FormInicio
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btLogin = new System.Windows.Forms.Button();
            this.btRegister = new System.Windows.Forms.Button();
            this.tbUsernameLI = new System.Windows.Forms.TextBox();
            this.tbPassLI = new System.Windows.Forms.TextBox();
            this.tbName = new System.Windows.Forms.TextBox();
            this.tbLN1 = new System.Windows.Forms.TextBox();
            this.tbLN2 = new System.Windows.Forms.TextBox();
            this.tbAge = new System.Windows.Forms.TextBox();
            this.tbUserNameR = new System.Windows.Forms.TextBox();
            this.tbPassR = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.btnConect = new System.Windows.Forms.Button();
            this.rb1 = new System.Windows.Forms.RadioButton();
            this.rb2 = new System.Windows.Forms.RadioButton();
            this.rb3 = new System.Windows.Forms.RadioButton();
            this.btndesc = new System.Windows.Forms.Button();
            this.btBuscar = new System.Windows.Forms.Button();
            this.btnConec = new System.Windows.Forms.Button();
            this.dataGridConect = new System.Windows.Forms.DataGridView();
            this.column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridConect)).BeginInit();
            this.SuspendLayout();
            // 
            // btLogin
            // 
            this.btLogin.Location = new System.Drawing.Point(12, 185);
            this.btLogin.Name = "btLogin";
            this.btLogin.Size = new System.Drawing.Size(100, 34);
            this.btLogin.TabIndex = 0;
            this.btLogin.Text = "login";
            this.btLogin.UseVisualStyleBackColor = true;
            this.btLogin.Visible = false;
            this.btLogin.Click += new System.EventHandler(this.btLogin_Click);
            // 
            // btRegister
            // 
            this.btRegister.Location = new System.Drawing.Point(6, 457);
            this.btRegister.Name = "btRegister";
            this.btRegister.Size = new System.Drawing.Size(102, 35);
            this.btRegister.TabIndex = 1;
            this.btRegister.Text = "registrarse";
            this.btRegister.UseVisualStyleBackColor = true;
            this.btRegister.Visible = false;
            this.btRegister.Click += new System.EventHandler(this.btRegister_Click);
            // 
            // tbUsernameLI
            // 
            this.tbUsernameLI.Location = new System.Drawing.Point(12, 116);
            this.tbUsernameLI.Name = "tbUsernameLI";
            this.tbUsernameLI.Size = new System.Drawing.Size(100, 20);
            this.tbUsernameLI.TabIndex = 2;
            this.tbUsernameLI.Visible = false;
            // 
            // tbPassLI
            // 
            this.tbPassLI.Location = new System.Drawing.Point(12, 152);
            this.tbPassLI.Name = "tbPassLI";
            this.tbPassLI.Size = new System.Drawing.Size(100, 20);
            this.tbPassLI.TabIndex = 3;
            this.tbPassLI.Visible = false;
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(6, 326);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(100, 20);
            this.tbName.TabIndex = 4;
            this.tbName.Visible = false;
            // 
            // tbLN1
            // 
            this.tbLN1.Location = new System.Drawing.Point(132, 326);
            this.tbLN1.Name = "tbLN1";
            this.tbLN1.Size = new System.Drawing.Size(100, 20);
            this.tbLN1.TabIndex = 5;
            this.tbLN1.Visible = false;
            // 
            // tbLN2
            // 
            this.tbLN2.Location = new System.Drawing.Point(6, 371);
            this.tbLN2.Name = "tbLN2";
            this.tbLN2.Size = new System.Drawing.Size(100, 20);
            this.tbLN2.TabIndex = 6;
            this.tbLN2.Visible = false;
            // 
            // tbAge
            // 
            this.tbAge.Location = new System.Drawing.Point(132, 371);
            this.tbAge.Name = "tbAge";
            this.tbAge.Size = new System.Drawing.Size(100, 20);
            this.tbAge.TabIndex = 7;
            this.tbAge.Visible = false;
            // 
            // tbUserNameR
            // 
            this.tbUserNameR.Location = new System.Drawing.Point(6, 417);
            this.tbUserNameR.Name = "tbUserNameR";
            this.tbUserNameR.Size = new System.Drawing.Size(100, 20);
            this.tbUserNameR.TabIndex = 8;
            this.tbUserNameR.Visible = false;
            // 
            // tbPassR
            // 
            this.tbPassR.Location = new System.Drawing.Point(132, 417);
            this.tbPassR.Name = "tbPassR";
            this.tbPassR.Size = new System.Drawing.Size(100, 20);
            this.tbPassR.TabIndex = 9;
            this.tbPassR.Visible = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(17, 68);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(115, 24);
            this.label1.TabIndex = 10;
            this.label1.Text = "inicar sesion";
            this.label1.Visible = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(15, 268);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 24);
            this.label2.TabIndex = 11;
            this.label2.Text = "registrarse";
            this.label2.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Nombre usuario";
            this.label3.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(20, 136);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(66, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Contrasenya";
            this.label4.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 310);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Nombre";
            this.label5.Visible = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(138, 310);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(75, 13);
            this.label6.TabIndex = 15;
            this.label6.Text = "Primer apellido";
            this.label6.Visible = false;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 355);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 13);
            this.label7.TabIndex = 16;
            this.label7.Text = "Segundo apellido";
            this.label7.Visible = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(139, 355);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(32, 13);
            this.label8.TabIndex = 17;
            this.label8.Text = "Edad";
            this.label8.Visible = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(13, 401);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(81, 13);
            this.label9.TabIndex = 18;
            this.label9.Text = "Nombre usuario";
            this.label9.Visible = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(139, 401);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(66, 13);
            this.label10.TabIndex = 19;
            this.label10.Text = "Contrasenya";
            this.label10.Visible = false;
            // 
            // btnConect
            // 
            this.btnConect.BackColor = System.Drawing.Color.YellowGreen;
            this.btnConect.Location = new System.Drawing.Point(12, 12);
            this.btnConect.Name = "btnConect";
            this.btnConect.Size = new System.Drawing.Size(113, 37);
            this.btnConect.TabIndex = 20;
            this.btnConect.Text = "Conectar";
            this.btnConect.UseVisualStyleBackColor = false;
            this.btnConect.Click += new System.EventHandler(this.btnConect_Click);
            // 
            // rb1
            // 
            this.rb1.AutoSize = true;
            this.rb1.Location = new System.Drawing.Point(403, 180);
            this.rb1.Name = "rb1";
            this.rb1.Size = new System.Drawing.Size(110, 17);
            this.rb1.TabIndex = 21;
            this.rb1.TabStop = true;
            this.rb1.Text = "Primera busqueda";
            this.rb1.UseVisualStyleBackColor = true;
            this.rb1.Visible = false;
            // 
            // rb2
            // 
            this.rb2.AutoSize = true;
            this.rb2.Location = new System.Drawing.Point(403, 217);
            this.rb2.Name = "rb2";
            this.rb2.Size = new System.Drawing.Size(118, 17);
            this.rb2.TabIndex = 22;
            this.rb2.TabStop = true;
            this.rb2.Text = "Segunda busqueda";
            this.rb2.UseVisualStyleBackColor = true;
            this.rb2.Visible = false;
            // 
            // rb3
            // 
            this.rb3.AutoSize = true;
            this.rb3.Location = new System.Drawing.Point(403, 255);
            this.rb3.Name = "rb3";
            this.rb3.Size = new System.Drawing.Size(112, 17);
            this.rb3.TabIndex = 23;
            this.rb3.TabStop = true;
            this.rb3.Text = "Tercera busqueda";
            this.rb3.UseVisualStyleBackColor = true;
            this.rb3.Visible = false;
            // 
            // btndesc
            // 
            this.btndesc.BackColor = System.Drawing.Color.Red;
            this.btndesc.Location = new System.Drawing.Point(403, 385);
            this.btndesc.Name = "btndesc";
            this.btndesc.Size = new System.Drawing.Size(115, 43);
            this.btndesc.TabIndex = 24;
            this.btndesc.Text = "Desconectar";
            this.btndesc.UseVisualStyleBackColor = false;
            this.btndesc.Visible = false;
            this.btndesc.Click += new System.EventHandler(this.btndesc_Click);
            // 
            // btBuscar
            // 
            this.btBuscar.Location = new System.Drawing.Point(420, 300);
            this.btBuscar.Name = "btBuscar";
            this.btBuscar.Size = new System.Drawing.Size(93, 32);
            this.btBuscar.TabIndex = 25;
            this.btBuscar.Text = "Buscar";
            this.btBuscar.UseVisualStyleBackColor = true;
            this.btBuscar.Visible = false;
            this.btBuscar.Click += new System.EventHandler(this.btBuscar_Click);
            // 
            // btnConec
            // 
            this.btnConec.Location = new System.Drawing.Point(624, 4);
            this.btnConec.Name = "btnConec";
            this.btnConec.Size = new System.Drawing.Size(96, 34);
            this.btnConec.TabIndex = 26;
            this.btnConec.Text = "Conectados";
            this.btnConec.UseVisualStyleBackColor = true;
            this.btnConec.Click += new System.EventHandler(this.btnConec_Click);
            // 
            // dataGridConect
            // 
            this.dataGridConect.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridConect.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.column1});
            this.dataGridConect.Location = new System.Drawing.Point(624, 44);
            this.dataGridConect.Name = "dataGridConect";
            this.dataGridConect.Size = new System.Drawing.Size(256, 409);
            this.dataGridConect.TabIndex = 27;
            // 
            // column1
            // 
            this.column1.HeaderText = "Nombre Jugador Conectado";
            this.column1.Name = "column1";
            // 
            // FormInicio
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(892, 540);
            this.Controls.Add(this.dataGridConect);
            this.Controls.Add(this.btnConec);
            this.Controls.Add(this.btBuscar);
            this.Controls.Add(this.btndesc);
            this.Controls.Add(this.rb3);
            this.Controls.Add(this.rb2);
            this.Controls.Add(this.rb1);
            this.Controls.Add(this.btnConect);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbPassR);
            this.Controls.Add(this.tbUserNameR);
            this.Controls.Add(this.tbAge);
            this.Controls.Add(this.tbLN2);
            this.Controls.Add(this.tbLN1);
            this.Controls.Add(this.tbName);
            this.Controls.Add(this.tbPassLI);
            this.Controls.Add(this.tbUsernameLI);
            this.Controls.Add(this.btRegister);
            this.Controls.Add(this.btLogin);
            this.Name = "FormInicio";
            this.Text = "FormInicio";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridConect)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btLogin;
        private System.Windows.Forms.Button btRegister;
        private System.Windows.Forms.TextBox tbUsernameLI;
        private System.Windows.Forms.TextBox tbPassLI;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.TextBox tbLN1;
        private System.Windows.Forms.TextBox tbLN2;
        private System.Windows.Forms.TextBox tbAge;
        private System.Windows.Forms.TextBox tbUserNameR;
        private System.Windows.Forms.TextBox tbPassR;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button btnConect;
        private System.Windows.Forms.RadioButton rb1;
        private System.Windows.Forms.RadioButton rb2;
        private System.Windows.Forms.RadioButton rb3;
        private System.Windows.Forms.Button btndesc;
        private System.Windows.Forms.Button btBuscar;
        private System.Windows.Forms.Button btnConec;
        private System.Windows.Forms.DataGridView dataGridConect;
        private System.Windows.Forms.DataGridViewTextBoxColumn column1;
    }
}