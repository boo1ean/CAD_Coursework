﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPF_GUI
{
    /// <summary>
    /// Interaction logic for LogWindow.xaml
    /// </summary>
    public partial class LogWindow : Window
    {
        public LogWindow()
        {
            InitializeComponent();
        }

        public void Print(string str)
        {
            LogViewer.Text += "> " + str + "\n";
        }

        private void LogWindow_OnClosing(object sender, CancelEventArgs e)
        {
//            StaticLoader.MainWindow.ShowLogViewer.IsChecked = false;
        }
    }
}
