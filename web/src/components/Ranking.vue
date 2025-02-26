<template>
    <div class="w-100 h-100 d-flex justify-center align-center flex-column">
        <v-card class="d-flex justify-center align-center flex-column pa-10 w-75">
            <v-card-title class="text-h2">
                最終結果
            </v-card-title>
            <div v-for="(user, index) in getSortedUser().slice(0, 3)">
                <v-card flat>
                    <div class="d-flex justify-center align-center flex-row">
                        <v-icon size="50" :color="getColor(index)">mdi-crown</v-icon>
                        <v-card-title class="text-h3">
                            {{ user.username }}({{ user.score }})
                        </v-card-title>
                        <v-icon size="50" :color="getColor(index)">mdi-crown</v-icon>
                    </div>
                </v-card>
            </div>

            <v-btn class="mt-10 w-25" color="success" @click="router.push('/')">終了</v-btn>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import router from '@/router';


type User = {
    username: string,
    isReady: boolean,
    judging: boolean,
    isSuccessed: boolean | null,
    score: number,
}

type Props = {
    users: User[]
}

const props = defineProps<Props>();
const getSortedUser = () => [...props.users].sort((a, b) => b.score - a.score);
const getColor = (rank: number): string => ["yellow-darken-2", "blue-grey-lighten-1", "brown-darken-2"][rank]
</script>