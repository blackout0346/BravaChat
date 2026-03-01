using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для PanelPage.xaml
    /// </summary>
    public partial class PanelPage : UserControl
    {
        public PanelPage()
        {
            InitializeComponent();
        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow Logins = new MainWindow();
            Logins.Show();
            
        }

        private void createchat_Click(object sender, RoutedEventArgs e)
        {
            AddUsers addUsers = new AddUsers();
            addUsers.Show();
        }
    }
}
