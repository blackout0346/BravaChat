using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.UI.WebControls;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для ItemContact.xaml
    /// </summary>
    public partial class ItemContact : UserControl
    {
        public int chatId { get; set; }

        public ItemContact(int id, string name)
        {
            InitializeComponent();
   
            this.chatId = id;
            this.UserName.Content  = name;
        }

   
    }
}
