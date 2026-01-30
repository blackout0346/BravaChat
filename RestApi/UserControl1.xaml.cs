using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Web.UI.WebControls;
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
    /// Логика взаимодействия для UserControl1.xaml
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        public string Text = null;
        struct Messages
        {
            public string msg { get; set; }
            public DateTime senddate { get; set; }
        }
        public UserControl1()
        {
            InitializeComponent();
        }

        private async void SetMessage()
        {
            Text = InputMsg.Text;
            Messages messages = new Messages() 
            {
                msg = Text
            };
            RestRequest restRequest = new RestRequest("/user/message", Method.Post);
            var jsonbody = JsonSerializer.Serialize(messages);
            restRequest.AddJsonBody(jsonbody);
            RestResponse response = await restClient.ExecuteAsync<Messages>(restRequest);
            if (response.StatusCode == System.Net.HttpStatusCode.OK)
            {
                System.Windows.Controls.Label label = new System.Windows.Controls.Label();
                label.Width = 500;
                label.Height = 500;
                label.Content = response.Content;
            }
            else
            {
                MessageBox.Show("Сообщение не отправилось");
            }
        }

        private void sendMessage_Click(object sender, RoutedEventArgs e)
        {
            SetMessage();
        }
    }
}
