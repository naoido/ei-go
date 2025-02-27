import requests

text = "The answer is"
lang = "en"

url = f"https://translate.google.com/translate_tts?ie=UTF-8&tl={lang}&q={text}&client=tw-ob"

headers = {
    "User-Agent": "Mozilla/5.0"
}
response = requests.get(url, headers=headers)

with open("google_translate_tts.mp3", "wb") as f:
    f.write(response.content)

print("音声ファイル 'google_translate_tts.mp3' を作成しました。")
