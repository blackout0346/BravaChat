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
    /// Логика взаимодействия для AddItemUser.xaml
    /// </summary>
    public partial class AddItemUser : UserControl
    {
        public event Action<int> OnaddUserClicked;
        public int PartnerId { get; set; }

        public AddItemUser(int id, string name)
        {
            InitializeComponent();

            this.PartnerId = id;
            this.UserName.Content = name;
        }

        private void addedUser_Click(object sender, RoutedEventArgs e)
        {
            OnaddUserClicked?.Invoke(PartnerId);
        }
    }
}
