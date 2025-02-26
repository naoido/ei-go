import requests
from fastapi import FastAPI, Response

app = FastAPI()

@app.get("/translate_tts")
async def google_translate_tts(text: str, lang: str = "en"):
    url = f"https://translate.google.com/translate_tts?ie=UTF-8&tl={lang}&q={text}&client=tw-ob"

    headers = {
        "User-Agent": "Mozilla/5.0"
    }
    response = requests.get(url, headers=headers)

    return Response(content=response.content, media_type="audio/mpeg")
