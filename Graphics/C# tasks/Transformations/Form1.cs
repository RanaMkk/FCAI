using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace transformations
{
    public partial class Form1 : Form
    {

        private double Dx = 0, Dy = 0;
        private double[,] Points = new double[3, 2];
        private double[,] Translation;
        private double[] Scalling;
        private double[] Reflection;
        private double[] Shearing;
        private double angle;
        public Form1()
        {
            InitializeComponent();
            chart1.Titles.Add("2D Transformations");
            Translation = new double[3, 3] { { 1, 0, Dx }, { 0, 1, Dy }, { 0, 0, 1 } };
            Scalling = new double[2];
            Reflection = new double[2];
            Shearing = new double[2];
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            chart1.Series["shape"].Points.Clear();
            textBox1.Text = " ";
            textBox2.Text = " ";
            textBox3.Text = " ";
            textBox4.Text = " ";
            textBox5.Text = " ";
            textBox6.Text = " ";
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                Translation[0, 2] = Convert.ToDouble(textBox12.Text);
                Translation[1, 2] = Convert.ToDouble(textBox11.Text);
                for (int i = 0; i < 3; i++)
                {
                    Points[i, 0] += Translation[0, 2];
                    Points[i, 1] += Translation[1, 2];
                    chart1.Series["translation"].Points.AddXY(Points[i, 0], Points[i, 1]);
                }
                chart1.Series["translation"].Points.AddXY(Points[0, 0], Points[0, 1]);
            }
            if (checkBox2.Checked == true)
            {
                Scalling[0] = Convert.ToDouble(textBox10.Text);
                Scalling[1] = Convert.ToDouble(textBox9.Text);
                for (int i = 0; i < 3; i++)
                {
                    Points[i, 0] *= Scalling[0];
                    Points[i, 1] *= Scalling[1];
                    chart1.Series["scaling"].Points.AddXY(Points[i, 0], Points[i, 1]);
                }
                chart1.Series["scaling"].Points.AddXY(Points[0, 0], Points[0, 1]);
            }
            if (checkBox5.Checked == true)
            {
                angle = Convert.ToDouble(textBox15.Text);
                for (int i = 0; i < 3; i++)
                {
                    double x = Points[i, 0];
                    double y = Points[i, 1];
                    x = Points[i, 0] * Math.Cos(angle) - Points[i, 1] * Math.Sin(angle);
                    y = Points[i, 0] * Math.Sin(angle) + Points[i, 1] * Math.Cos(angle);
                    chart1.Series["rotation"].Points.AddXY(x, y);
                    Points[i, 0] = x;
                    Points[i, 1] = y;
                }
                chart1.Series["rotation"].Points.AddXY(Points[0, 0], Points[0, 1]);
            }
            if (checkBox4.Checked == true)
            {
                Reflection[0] = Convert.ToDouble(textBox8.Text);
                Reflection[1] = Convert.ToDouble(textBox7.Text);
                for (int i = 0; i < 3; i++)
                {
                    Points[i, 0] *= Reflection[0];
                    Points[i, 1] *= Reflection[1];
                    chart1.Series["refliction"].Points.AddXY(Points[i, 0], Points[i, 1]);
                }
                chart1.Series["refliction"].Points.AddXY(Points[0, 0], Points[0, 1]);
            }
            if (checkBox3.Checked == true)
            {
                Shearing[0] = Convert.ToDouble(textBox14.Text);
                Shearing[1] = Convert.ToDouble(textBox13.Text);
                for (int i = 0; i < 3; i++)
                {
                    Points[i, 0] += Shearing[0] * Points[i, 1];
                    Points[i, 1] += Shearing[1] * Points[i, 0];
                    chart1.Series["shearing"].Points.AddXY(Points[i, 0], Points[i, 1]);
                }
                chart1.Series["shearing"].Points.AddXY(Points[0, 0], Points[0, 1]);
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textBox12_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox11_TextChanged(object sender, EventArgs e)
        {

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textBox10_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox9_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox15_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox8_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textBox14_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox13_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            chart1.Series["translation"].Points.Clear();
            chart1.Series["scaling"].Points.Clear();
            chart1.Series["rotation"].Points.Clear();
            chart1.Series["refliction"].Points.Clear();
            chart1.Series["shearing"].Points.Clear();
            textBox7.Text = " ";
            textBox8.Text = " ";
            textBox9.Text = " ";
            textBox10.Text = " ";
            textBox11.Text = " ";
            textBox12.Text = " ";
            textBox13.Text = " ";
            textBox14.Text = " ";
            textBox15.Text = " ";

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Points[0, 0] = Convert.ToDouble(textBox1.Text);
            Points[0, 1] = Convert.ToDouble(textBox2.Text);
            Points[1, 0] = Convert.ToDouble(textBox3.Text);
            Points[1, 1] = Convert.ToDouble(textBox4.Text);
            Points[2, 0] = Convert.ToDouble(textBox6.Text);
            Points[2, 1] = Convert.ToDouble(textBox5.Text);
            for (int i = 0; i < 3; i++)
            {
                chart1.Series["shape"].Points.AddXY(Points[i, 0], Points[i, 1]);
            }
            chart1.Series["shape"].Points.AddXY(Points[0, 0], Points[0, 1]);
        }
    }
}
