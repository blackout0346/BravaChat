using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using RestSharp;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Text.Json.Serialization;
using System.Windows.Media;
using System.Windows.Threading;
namespace RestApi
{
    /// <summary>
    /// Логика взаимодействия для UserControl1.xaml
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        RestClient restClient = new RestClient("http://127.0.0.1:18080");
        private int Myid;
        private int ChatId;
        private DispatcherTimer dispatcherTimer;
        public class MessageItem
        {

            [JsonPropertyName("Login")]
            public string login { get; set; }

            [JsonPropertyName("UserId")]
            public int SenderId { get; set; }

            [JsonPropertyName("message")]
            public string msg { get; set; }
            [JsonPropertyName("SendDate")]
            public string senddate { get; set; }
            public HorizontalAlignment alignment { get; set; }
            public SolidColorBrush bubbleColor { get; set; }
        }
        
        public UserControl1(int myId, int chatId)
        {
            InitializeComponent();
            Myid = myId;
            ChatId = chatId;
            LoadMessages();
            dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Interval = TimeSpan.FromSeconds(2);
            dispatcherTimer.Tick += (s, e) => LoadMessages();
            dispatcherTimer.Start();
        }

        private async void LoadMessages()
        {
            try
            {
                RestRequest request = new RestRequest($"/users/messages/chat/{ChatId}", Method.Get);
                var response = await restClient.ExecuteAsync<List<MessageItem>>(request);
                if (response.IsSuccessStatusCode && response.Data != null)
                {
                    if (MessagesList.Items.Count != response.Data.Count)
                    {
                        MessagesList.Items.Clear();
                        foreach (var m in response.Data)
                        {
                            //if (m.SenderId == Myid)
                            //{
                            //    m.alignment = HorizontalAlignment.Right;
                            //    m.bubbleColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#DCF8C6"));
                            //}
                            //else
                            //{
                            //    m.alignment = HorizontalAlignment.Left;
                            //    m.bubbleColor = new SolidColorBrush(Colors.AliceBlue);
                            //}
                            MessagesList.Items.Add(m);
                        }
                        if (MessagesList.Items.Count > 0)
                        {
                            MessagesList.ScrollIntoView(MessagesList.Items[MessagesList.Items.Count - 1]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("error");
            }
        }
        
        private async void SetMessage()
        {
            var messages = new
            {
                userId = Myid,
                chatId = ChatId,
                message = InputMsg.Text,
                replyId = 0,
                forwardId = 0
            };
            RestRequest restRequest = new RestRequest("/users/messages", Method.Post);
            restRequest.AddJsonBody(messages);
            var response = await restClient.ExecuteAsync(restRequest);
            if (response.IsSuccessStatusCode)
            {
                InputMsg.Clear();
                LoadMessages();
            }
            else
            {
                MessageBox.Show($"Ошибка: {response.StatusCode}\nОтвет сервера: {response.Content}");
            }
        }
     

        private void sendMessage_Click(object sender, RoutedEventArgs e)
        {

            SetMessage();
        }
    }
}