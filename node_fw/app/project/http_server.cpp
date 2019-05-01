#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "common/http_client.h"

static HttpServer server;
static Timer trigger_timer;

static void onIndex(HttpRequest &request, HttpResponse &response)
{
    debugf("On INDEX\n");
    response.code = HTTP_STATUS_OK;
    response.sendString("<html><body><h1>Cool!</h1></body></html>");
}

uint8_t trigger_enabled[18] = {false};
uint8_t trigger_sent[18] = {false};
uint8_t trigger_value[18] = {LOW};

void trigger_task()
{
    for (int i = 0; i < 18; i++)
    {
        if (trigger_enabled[i])
        {
            int value = digitalRead(i);
            if (digitalRead(i) == trigger_value[i])
            {
                if (trigger_sent[i] == false)
                {
                    sendNodeUpdate("{"
                                   "\"pin\":" +
                                   String(i) +
                                   ", \"value\":" + String(value) +
                                   "}");
                    trigger_sent[i] = true;
                }
            }
            else
            {
                trigger_sent[i] = false;
            }
        }
    }
}

void onConfigurePin(HttpRequest &request, HttpResponse &response)
{
    debugf("onConfigurePin\n");

    String body = request.getBody();

    int pin = request.getQueryParameter("pin").toInt();
    int direction = request.getQueryParameter("direction").toInt();

    if (direction == 1)
    {
        pinMode(pin, INPUT);
    }
    else if (direction == 0)
    {
        pinMode(pin, OUTPUT);
    }

    response.code = HTTP_STATUS_OK;
    response.sendString("OK");
}

void onReadPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onReadPin\n");
    int pin = request.getQueryParameter("pin").toInt();
    int value = digitalRead(pin);

    response.code = HTTP_STATUS_OK;
    response.sendString("{"
                        "\"value\":" +
                        String(value) +
                        ",\"pin\":" + String(pin) +
                        "}");
}

void onWritePin(HttpRequest &request, HttpResponse &response)
{
    debugf("onWritePin\n");
    int pin = request.getQueryParameter("pin").toInt();
    int value = request.getQueryParameter("value").toInt();

    digitalWrite(pin, value);

    response.code = HTTP_STATUS_OK;
    response.sendString("{"
                        "\"value\":" +
                        String(value) +
                        ",\"pin\":" + String(pin) +
                        "}");
}

void onAddTriggerPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onAddTriggerPin");
    int pin = request.getQueryParameter("pin").toInt();
    int value = request.getQueryParameter("value").toInt();

    if (pin >= 18 || pin < 0)
    {
        debugf("Error Unknown Pin\r\n");
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"error\": \"Unknown Pin\""
                            "}");
    }
    else
    {
        debugf("Trigger Added \r\n");
        trigger_enabled[pin] = true;
        trigger_value[pin] = value;
        trigger_sent[pin] = false;
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"status\": \"OK\""
                            "}");
    }
}

void onRemoveTriggerPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onRemoveTriggerPin");
    int pin = request.getQueryParameter("pin").toInt();

    if (pin >= 18)
    {
        debugf("Error Unknown Pin\r\n");
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"error\": \"Unknown Pin\""
                            "}");
    }
    else
    {
        debugf("Trigger Removed \r\n");
        trigger_enabled[pin] = false;
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"status\": \"OK\""
                            "}");
    }
}

static void onDefault(HttpRequest &request, HttpResponse &response)
{
    debugf("Unknown path requested: %s\n", request.getPath().c_str());
    response.notFound();
}

void startWebServer(void)
{
    debugf("==== STARTING WEB SERVER ====\n");
    server.listen(80);
    server.addPath("/", onIndex);
    server.addPath("/configure-pin", onConfigurePin);
    server.addPath("/read-pin", onReadPin);
    server.addPath("/write-pin", onWritePin);
    server.addPath("/add-trigger", onAddTriggerPin);
    server.addPath("/remove-trigger", onRemoveTriggerPin);

    server.setDefaultHandler(onDefault);

    trigger_timer.initializeMs(100, trigger_task).start();
}