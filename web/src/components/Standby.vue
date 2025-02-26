<template>
    <div class="w-100 h-100 d-flex justify-center align-center flex-column">
        <v-card class="w-75 d-flex justify-center align-center flex-row py-10">
            <div class="w-50 d-flex justify-center flex-column align-center">
                <div class="mb-5">
                    <v-card-title>ルームID</v-card-title>
                    <v-card-subtitle>
                        {{ roomId }}
                    </v-card-subtitle>
                </div>
                <vue-qrcode :value="roomId" :options="{ width: 200, margin: 1 }" class="mb-4" />
                <v-btn :color="isReady ? 'success' : 'blue-grey-lighten-'" :disabled="!isReady">スタート</v-btn>
            </div>
            <div class="w-100">
                <h1>参加ユーザー({{ users.length }}/{{ users.length }})</h1>
                <v-virtual-scroll
                    :height="400"
                    :items="users"
                >
                    <template v-slot:default="{ item }">
                        <v-card flat class="d-flex justify-start align-center">
                            <v-icon :color="true ? 'success' : 'error'">{{ true ? "mdi-check-bold" : "mdi-close" }}</v-icon>
                            <v-card-text style="font-size: 20px;">
                                {{ item }}
                            </v-card-text>
                        </v-card>
                    </template>
                </v-virtual-scroll>
            </div>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import VueQrcode from "@chenfengyuan/vue-qrcode";
import { ref } from "vue";

const props = defineProps({
    roomId: { type: String, required: true }
})

const roomId = props.roomId;
const isReady = ref(true);
const users = ref<string[]>(["ユーザー1", "ユーザー2", "ユーザー3", "ユーザー4", "ユーザー1", "ユーザー2", "ユーザー3", "ユーザー4"])
</script>