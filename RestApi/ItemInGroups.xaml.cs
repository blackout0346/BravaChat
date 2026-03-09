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
    /// Логика взаимодействия для ItemInGroups.xaml
    /// </summary>
    public partial class ItemInGroups : UserControl
    {
        public int UserId { get; set; }
        public string login { get; set; }
        public event Action<int> OnUserChecked;
        public event Action<int> OnUserUnChecked;

        public ItemInGroups(int Userid, string login)
        {
            InitializeComponent();
            this.UserId = Userid;
            this.login = login;
            UserName.Content = login;
        }

        private void addedUser_Click(object sender, RoutedEventArgs e)
        {
           
        }

        private void addedUser_Checked(object sender, RoutedEventArgs e)
        {
            OnUserChecked?.Invoke(UserId);
        }

        private void addedUser_Unchecked(object sender, RoutedEventArgs e)
        {
            OnUserUnChecked?.Invoke(UserId);
        }
    }
}
